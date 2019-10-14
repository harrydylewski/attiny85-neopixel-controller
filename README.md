# attiny85-neopixel-controller

PB2 = button (connected between gnd and PB2) to switch modes 
PB3 = button (connected between gnd and PB3) to switch timing
PB4 = output for neopixels
this project needs the attiny85 to use it's 8mhz internal crystal

The pixel_tx_progmem() function will output anything in a progmem array.
I put all the colors in progmem array to make things easier to manage.
I change some of the values based off a gamma correction program,
you can read and learn more here
https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix

Version 1B - Feature set
2 modes - rainbow cycling and color cycling
Changing Times - can switch between update times to between 16,32,64,256ms
Automatically saves any changes, although you should give it a second between
button presses for eeprom to save properly.

