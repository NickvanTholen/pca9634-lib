/*
    PCA9634.cpp - Library for NXP PCA9634 chip.
    Created by Nick van Tholen, June 2, 2020.
    Released for Emit IT.
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
    _brightness[pin] = brightness;
    pinType(2, pin);
    int interval = time / brightness;
    for (int i = 0; i <= brightness; i++)
    {
        chanPwm(pin, i);
        delay(interval);
    }
}

// Fade out for single led channel
void PCA9634::fadeOut(uint8_t pin, int time, uint8_t brightness){
    pinType(2, pin);
    int interval = time / (_brightness[pin] - brightness);
    for (int i = _brightness[pin]; i >= brightness; i--){
        chanPwm(pin, i);
        delay(interval);
    }
    _brightness[pin] = brightness;
}

// Pwm function for each individual channel
void PCA9634::pwm(uint8_t pin, uint8_t value){
    pinType(2, pin);
    chanPwm(pin, value);
}

// Private functions //

// Sets a channel or all channels to different state
void PCA9634::pinType(uint8_t type, uint8_t pin, bool all)
{
    uint8_t dataType;
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
        if(pin<4){
            bitClear(_LEDOUT0Register, (pin*2));
            bitClear(_LEDOUT0Register, (pin*2+1));
            writeReg(LEDOUT0, _LEDOUT0Register);
            break;
        }
        if(pin>=4){
            pin -= 4;
            bitClear(_LEDOUT1Register, (pin*2));
            bitClear(_LEDOUT1Register, (pin*2+1));
            writeReg(LEDOUT1, _LEDOUT1Register);
            break;
        }
        break;
    case 1:
        if(all){
            writeReg(LEDOUT0, LED_ON_ALL);
            writeReg(LEDOUT1, LED_ON_ALL);
            break;
        }
        if(pin<4){
            bitSet(_LEDOUT0Register, (pin*2));
            bitClear(_LEDOUT0Register, (pin*2+1));
            writeReg(LEDOUT0, _LEDOUT0Register);
            break;
        }
        if(pin>=4){
            pin -= 4;
            bitSet(_LEDOUT1Register, (pin*2));
            bitClear(_LEDOUT1Register, (pin*2+1));
            writeReg(LEDOUT1, _LEDOUT1Register);
            break;
        }
        break;
    case 2:
        if(all){
            writeReg(LEDOUT0, LED_PWM_ALL);
            writeReg(LEDOUT1, LED_PWM_ALL);
            break;
        }
        if(pin<4){
            bitClear(_LEDOUT0Register, (pin*2));
            bitSet(_LEDOUT0Register, (pin*2+1));
            writeReg(LEDOUT0, _LEDOUT0Register);
            break;
        }
        if(pin>=4){
            pin -= 4;
            bitClear(_LEDOUT1Register, (pin*2));
            bitSet(_LEDOUT1Register, (pin*2+1));
            writeReg(LEDOUT1, _LEDOUT1Register);
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