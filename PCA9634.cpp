/*
    PCA9634.cpp - Library for NXP PCA9634 chip.
    Created by Nick van Tholen, June 2, 2020.
    Released for Emit IT.
    Last modified, 23 June, 2020.
*/
#include "Arduino.h"
#include "PCA9634.h"

// Constructor //

// Initialize the Library
PCA9634::PCA9634(uint8_t adress, uint8_t oePin){
    _oepin = oePin;
    _addr = adress;
    pinMode(oePin, OUTPUT);
}

// Public //

// Uses a I2C adress reset
void PCA9634::softReset()
{
    Wire.beginTransmission(0x03);
    Wire.write(0xA5);
    Wire.write(0x5A);
    Wire.endTransmission();
}

// Sets up the whole chip and I2C connection
void PCA9634::begin()
{
    digitalWrite(_oepin, LOW);
    Wire.begin();
    delay(10);
    writeReg(0x00, 0x01);
    delayMicroseconds(500);
    writeReg(0x01, 0x14);
    delay(10);
}

// Writes to a register
uint8_t PCA9634::writeReg(uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.write(val);
    return Wire.endTransmission();
}

// Reads a register
uint8_t PCA9634::readReg(uint8_t reg)
{
    Wire.beginTransmission(_addr);
    Wire.write(reg & 0x1F);
    Wire.endTransmission();
    Wire.requestFrom(_addr, (uint8_t)1);
    return Wire.read();
}

// Uses the output enable pin to enable and disable all channels, doesn't effect the previous state.
void PCA9634::enabled(bool state)
{
    if(state == true){
        digitalWrite(_oepin, LOW);
    }
    else{
        digitalWrite(_oepin, HIGH);
    }
}

// Turn a channel on
void PCA9634::on(uint8_t pin){
    pinType(1, pin);
    chanPwm(pin, 255);
}

// Turn a channel off
void PCA9634::off(uint8_t pin){
    pinType(0, pin);
    chanPwm(pin, 0);
}

// Turn all channels on
void PCA9634::allOn(){
    pinType(1, 0, true);
    for (int i = 0; i < 8; i++){
        chanPwm(i, 255);
    }
}

// Turn all channels off
void PCA9634::allOff(){
    pinType(0, 0, true);
    for (int i = 0; i < 8; i++){
        chanPwm(i, 0);
    }
}

// Fade in for single led channel
void PCA9634::fadeIn(uint8_t pin, int time, uint8_t brightness){
    pinType(2, pin);
    int interval = time / brightness;
    for (int i = 0; i <= brightness; i++)
    {
        chanPwm(pin, i);
        delay(interval);
    }
    if (brightness == 255){
        pinType(1, pin);
    }
}

// Fade out for single led channel
void PCA9634::fadeOut(uint8_t pin, int time, uint8_t brightness){
    uint8_t regValue;
    pinType(2, pin);
    regValue = readReg(pin + 2);
    int interval = time / (regValue - brightness);
    for (int i = regValue; i >= brightness; i--){
        chanPwm(pin, i);
        delay(interval);
    }
    if (brightness == 0){
        pinType(0, pin);
    }
}

// Pwm function for each individual channel
void PCA9634::pwm(uint8_t pin, uint8_t value){
    pinType(2, pin);
    chanPwm(pin, value);
}

// Check if a led is Off, On or PWM modus
uint8_t PCA9634::ledStatus(uint8_t pin){
    uint8_t regValue;
    bool first, second;
    if (pin < 4){
        regValue = readReg(LEDOUT0);
        first = bitRead(regValue, (pin*2));
        second = bitRead(regValue, (pin*2+1));
    }
    else {
        pin -= 4;
        regValue = readReg(LEDOUT1);
        first = bitRead(regValue, (pin*2));
        second = bitRead(regValue, (pin*2+1));
    }
    if (!first && !second){
        return 0;
    }
    else if(first && !second){
        return 1;
    }
    else if(!first && second){
        return 2;
    }
}

// Check the PWM value of a channel
uint8_t PCA9634::pwmStatus(uint8_t pin){
    return readReg(pin + 2);
}

// Private functions //

// Sets a channel or all channels to different state
void PCA9634::pinType(uint8_t type, uint8_t pin, bool all)
{
    uint8_t dataType, regValue;
    if(type > 2){
        type = 0;
    }
    switch (type)
    {
    case 0:
        if(all){
            writeReg(LEDOUT0, LED_OFF_ALL);
            writeReg(LEDOUT1, LED_OFF_ALL);
            break;
        }
        else if(pin<4){
            regValue = readReg(LEDOUT0);
            bitClear(regValue, (pin*2));
            bitClear(regValue, (pin*2+1));
            writeReg(LEDOUT0, regValue);
            break;
        }
        else if(pin>=4){
            pin -= 4;
            regValue = readReg(LEDOUT1);
            bitClear(regValue, (pin*2));
            bitClear(regValue, (pin*2+1));
            writeReg(LEDOUT1, regValue);
            break;
        }
        break;
    case 1:
        if(all){
            writeReg(LEDOUT0, LED_ON_ALL);
            writeReg(LEDOUT1, LED_ON_ALL);
            break;
        }
        else if(pin<4){
            regValue = readReg(LEDOUT0);
            bitSet(regValue, (pin*2));
            bitClear(regValue, (pin*2+1));
            writeReg(LEDOUT0, regValue);
            break;
        }
        else if(pin>=4){
            pin -= 4;
            regValue = readReg(LEDOUT1);
            bitSet(regValue, (pin*2));
            bitClear(regValue, (pin*2+1));
            writeReg(LEDOUT1, regValue);
            break;
        }
        break;
    case 2:
        if(all){
            writeReg(LEDOUT0, LED_PWM_ALL);
            writeReg(LEDOUT1, LED_PWM_ALL);
            break;
        }
        else if(pin<4){
            regValue = readReg(LEDOUT0);
            bitClear(regValue, (pin*2));
            bitSet(regValue, (pin*2+1));
            writeReg(LEDOUT0, regValue);
            break;
        }
        else if(pin>=4){
            pin -= 4;
            regValue = readReg(LEDOUT1);
            bitClear(regValue, (pin*2));
            bitSet(regValue, (pin*2+1));
            writeReg(LEDOUT1, regValue);
            break;
        }
        break;
    }
}

// Writes a pwm value to a channel
void PCA9634::chanPwm(uint8_t channel, uint8_t value)
{
    channel += 2;
    writeReg(channel, value);
}