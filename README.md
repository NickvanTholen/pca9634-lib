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
* **PinType** - *your_name*.pinType(type, pin, all), enables you to set all or a single output to off, on or pwm-mode. To set all outputs set the all value to true, if nothing is enterd here it defaults to false. Type selects the mode for the pin, it uses the int value after the mode off(0), on(1), pwm(2). The pin variable needs a value between 0 and 7 to select the pin you want to control.
* **ChanPWM** - *your_name*.chanPwm(channel, value), enables you to set the pwm value for each channel individualy. The channel variable needs a value between 0 and 7 to select the pin you want to control. The value variable needs one byte of data to set the dutycycle of the selected led channel.
* **SoftReset** - *your_name*.softReset(), this function enables the software resetting of the PCA9634 chip. **Be careful with this function as it will reset all registers!**

## Changelog
* *07-06-2020* The initial setup of this library.
* *07-06-2020* Added a keywords.txt file and example for blinking leds with the pintype function.

## Future updates
* Fade in/out function
* Group pwm
* Blinking of leds
* Write different value to all leds