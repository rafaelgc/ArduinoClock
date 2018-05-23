# ArduinoClock

## Components

- 1x Arduino board (Uno, Mini Pro)
- 8x 330Ω resistors.
- 3x high value resistor (e.g.: 470000Ω).
- 4x 390Ω resistors.
- 4x 7-segments display. Common cathode.
- 3x buttons.
- 4x 2N5551 NPN transistors.
- 1x CD4511BE BCD-to-7-segment decoder.
- 1x buzzer.

## Scheme
![](scheme.png)

## Source code
Download it [here](https://github.com/rafaelgc/ArduinoClock/archive/master.zip).

## Configuration
There is a configuration section in the main file (`ArduinoClock.ino`). Before loading the code to your board make sure that pins are configured properly.

## Videos
### 2018
Running in an Arduino Mini Pro, it's build on a bakelite board. Instead of using four different 7-segments display I use a four digit 7-segment display: [Youtube video](https://youtu.be/82Oy63iAcYY)

### 2015
Running in an Arduino One, unlike the 2018 version, it has an additional led that is on when the alarm is on:
[Youtube video](https://www.youtube.com/watch?v=3Vp3RKkvZ58)
