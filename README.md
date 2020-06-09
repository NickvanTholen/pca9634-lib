# README
This is the readme for the PCA9634 library, it contains some usefull information on how to use the libraray.

## INFO
This is the library that is used to control the PCA9634 chip from NXP semiconductors. It is based on the I2C protocol and contains some usefull functions.

## Functions
### Setup
The only thing that you will need to include is the **Wire.h** library, this library will start the I2C protocol automaticly on the standard I2C pins of the microcontroller.

### Actual functions
* **Constructor** - PCA9634 *your_name*(adress, oePin), adress is the I2C adress, oePin is the pin where output enable (OE) is connected.
* **Begin** - *your_name*.begin(), set up and begin the I2C connection, set OE as output and enables the chip, sets the register to the right values.
* **Enabled** - *your_name*.enabled(state), sets the OE to turn on or off the outputs, requires a boolean value, if true the chip is enabled, if false the chip is disabled.
* **WriteReg** - *your_name*.writeReg(reg, val), enables you to write costum values to a register in the chip, reg is the hex value of the register you want to write to. The val is the value you want to write to the specified register. Also returns the I2C status as an integer for debuging purposes.
* **On** - *your_name*.on(pin), enables you to turn on a channel, pin variable needs to be an value between 0 and 7.
* **Off** - *your_name*.off(pin), enables you to turn off a channel, pin variable needs to be an value between 0 and 7.
* **allOn** - *your_name*.allOn(), enables you to turn all channels on.
* **allOf** - *your_name*.allOff(), enables you to turn all channels off.
* **Pwm** - *your_name*.pwm(pin, value), enables you to write a pwm value to a single channel, pin variable needs to be an value between 0 and 7, value variable uses one byte.
* **FadeIn** - *your_name*.fadeIn(pin, time, brightness), enables you to fade in a led channel, pin variable needs to be an value between 0 and 7. The time variable is the total time for the fade in in milliseconds, it expects a whole number. The brightness variable is the brightness to fade to, it expects a one byte of data and defaults to 255 if no value is given.
* **FadeOut** - *your_name*.fadeOut(pin, time, brightness), enables you to fade out a led channel, pin variable needs to be an value between 0 and 7. The time variable is the total time for the fade out in milliseconds, it expects a whole number. The brightness variable is the brightness to fade to, it expects a one byte of data and defaults to 0 if no value is given.
* **SoftReset** - *your_name*.softReset(), this function enables the software resetting of the PCA9634 chip. **Be careful with this function as it will reset all registers!**

### Removed functions
* **PinType** - *your_name*.pinType(type, pin, all), enables you to set all or a single output to off, on or pwm-mode. To set all outputs set the all value to true, if nothing is enterd here it defaults to false. Type selects the mode for the pin, it uses the int value after the mode off(0), on(1), pwm(2). The pin variable needs a value between 0 and 7 to select the pin you want to control. **Removed**
* **ChanPWM** - *your_name*.chanPwm(channel, value), enables you to set the pwm value for each channel individualy. The channel variable needs a value between 0 and 7 to select the pin you want to control. The value variable needs one byte of data to set the dutycycle of the selected led channel. **Removed**

## Changelog
* *07-06-2020* The initial setup of this library.
* *07-06-2020* Added a keywords.txt file and example for blinking leds with the pintype function.
* *09-06-2020* Moved pintype and ChanPWM function to private and added some new functions like: on, off, allOn, allOff, pwm, fadeIn & fadeOut. Also added some basic comments to the cpp file. Added removed functions to readme and added new functions to keywords file.

## Future updates
* Fade in/out function **Done**
* Group pwm
* Blinking of leds
* Write different value to all leds