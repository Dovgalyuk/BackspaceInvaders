# BackspaceInvaders
Game for Arduino and LED display 64x64

This project uses Adafruit-like display:
https://learn.adafruit.com/32x16-32x32-rgb-led-matrix

It requires the following Arduino connections:

Display-Arduino
* R1 - 2
* G1 - 3
* B1 - 4
* R2 - 5
* G2 - 6
* B2 - 7
* A - A0
* B - A1
* C - A2
* D - A3
* LAT - 10
* CLK - 8
* OE - 9

Buttons - Arduino
* LEFT - 11
* RIGHT - 12
* FIRE - 13

Lower part of display is connected through upper one as follows:
```
LLLLUUUU
LLLLUUUU
```
This is logically 128x32 display.
