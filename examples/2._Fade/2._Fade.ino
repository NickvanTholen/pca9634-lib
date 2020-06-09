// Example to fade in and fade out each led channel
// Created by: Nick van Tholen
// Date: 09-06-2020

// Libraries
#include <Wire.h>
#include <PCA9634.h>

// Setup of the PCA9634 with adress 0x28 and output enable pin 15.
PCA9634 ledDriver(0x28, 15);

void setup() {
  // Sets up the I2C connection and sets the values of the mode registers to the right values.
  ledDriver.begin();
  // Turn all leds on 1 for type, 0 for pin wich doesn't matter because all bool is set to true.
  ledDriver.allOn();
  // Delay for 2,5 seconds
  delay(2500);
  // Turn all leds off 0 for type, 0 for pin wich doesn't matter because all bool is set to true.
  ledDriver.allOff();
}

void loop() {
  // For loop for fade in and fade out each led channel on the chip
  for(int i = 0; i < 8; i++){
    // Turns on the led, last bool element is not needed because it defaults to false
    ledDriver.fadeIn(i, 1000);
    // Delay for 1 second
    delay(1000);
    // Turns off the led, last bool element is not needed because it defaults to false
    ledDriver.fadeOut(i, 1000);
  }
}
