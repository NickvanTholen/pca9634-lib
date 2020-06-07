/*
    PCA9634.h - Library for NXP PCA9634 chip.
    Created by Nick van Tholen, June 2, 2020.
    Released for Emit IT.
*/
#ifndef pca9634
#define pca9634

#define LED_OFF_ALL 0x00
#define LED_ON_ALL 0x55
#define LED_PWM_ALL 0xAA
#define LEDOUT0 0x0C
#define LEDOUT1 0x0D

#include <Arduino.h>
#include <Wire.h>

class PCA9634
{
    public:
        PCA9634(uint8_t adress, uint8_t oePin);
        void softReset();
        void begin();
        uint8_t writeReg(uint8_t reg, uint8_t val);
        void chanPwm(uint8_t channel, uint8_t value);
        void enabled(bool state);
        void pinType(uint8_t type, uint8_t pin, bool all=false);
    private:
        uint8_t _addr;
        uint8_t _oepin;
        uint8_t _LEDOUT0Register = 0x00;
        uint8_t _LEDOUT1Register = 0x00;
};

#endif
