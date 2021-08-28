# Infra-red remote to keyboard input converter

For as long as I can remember, setting up an infra-red remote control on Linux
has been a disaster.

This time, I decided to just not do that. Instead, I'm using a microcontroller
to receive the IR signals and simulate keyboard pressed by pretending to be a
keyboard.

The keyboard presses map to the keyboard controls for
[Kodi](https://kodi.wiki/view/Keyboard_controls).

## Hardware

You will need:

1. A microcontroller with USB
2. A TL1838 or similar infra-red connector
3. A suitable remote

You can probably do this with any board that the Arduino IDE supports. I used
an ATmega32u8 Pro Micro clone.

Connect the TL1838 to the board as follows:

- OUT to digital pin 2
- GND to ground
- VCC to VCC

For the remote controller, I used a One For All Contour model number URC1210,
and programmed it with code 1840. If you have an LG TV, this might clash,
and you might have to try something else, set the `DEBUG` constant and make
your own mapping. Not all codes provide a working unique code for every button,
and it might take trial and error to find one that works. Some protocols
seem to work more reliably that others, so you might need to try a few codes
before you find something that works well.
