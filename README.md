# Sinclair ZX Computer Projects 

## Tek AVR
Tek is a ZX Spectrum keyboard emulator based on an AVR microcontroller and some glue logic chips.
It works by converting the PS/2 keystrokes into a bitmap representation of the keyboard matrix and updating its outputs with the corresponding keys pressed depending on the state of lines A8..A15 each time the Z80 reads the port 0xFE.
The circuit is heavilly based on ATMega88 but any Arduino with enough pins should do it. 
Development documented in my personal blog at the months of 
[april](http://danjovic.blogspot.com/2014/04) 
[may](http://danjovic.blogspot.com/2014/05) 
[june](http://danjovic.blogspot.com/2014/06) 
[july](http://danjovic.blogspot.com/2014/07) 
[august](http://danjovic.blogspot.com/2014/08) 
, 2014.

## Tek
Generic matrix keyboard emulator. Based on PIC 16F688 can be compiled on SDCC or in CCS C.  [Hackaday.io project page](https://hackaday.io/project/6140-tek-ps2-to-matrix-keyboard-adapteremulator)

## Tek V2
Provides quick and dirty matrix keyboard emulator to complement ZX-80/81 and ZX Spectrum boards.  [Hackaday.io project page](https://hackaday.io/project/166917-tek-v2)

## paddle
Adapter for using Sega Master System [HPD-200](https://segaretro.org/Paddle_Control) paddle controller with [Kempston] (https://en.wikipedia.org/wiki/Kempston_Interface) interface. 
Test programs are provided for ZX81 and Speccy. [Hackaday.io page](https://hackaday.io/page/6399-sms-paddles-on-sinclair-zx-computers)
