# WLED DIP-Switches

This usermod is used to connect a 10-bit DIP-Switch using a mutliplexer to an ESP32 running WLED.
Using the DIP-Switches it is possible to switch between DMX- and Preset-Mode (bit 10) and selecting the Address (DMX) or the ID (Preset) accordingly (bits 0-9).

## Installation 

Compile and upload after adding `-D USERMOD_DIPSWITCH` to `build_flags` of your PlatformIO environment or use `#define USERMOD_DIPSWITCH` in `myconfig.h`.

## Hardware

I tested this usermod using an `CD74HC4067-SM96`. I added the `DIPSwitchUsermod_INVERT` build flag, as I needed to invert the connection between the Multiplexer and the DIP-Switches (I9-SW1, I8-SW2, ..., I0-SW10), as shown in the tested setup.

## Tested setup

| Supply |    CD74HC4067   | ESP32 | 10-bit DIP |
|:------:|:---------------:|:-----:|:----------:|
|   GND  | GND, E#, Common |  GND  |            |
|   3V3  |       VCC       |  VCC  |            |
|        |        S0       |  IO27 |            |
|        |        S1       |  IO26 |            |
|        |        S2       |  IO12 |            |
|        |        S3       |  IO33 |            |
|        |       SIG       |  IO25 |            |
|        |        I0       |       |    SW10    |
|        |        I1       |       |     SW9    |
|        |        I2       |       |     SW8    |
|        |        I3       |       |     SW7    |
|        |        I4       |       |     SW6    |
|        |        I5       |       |     SW5    |
|        |        I6       |       |     SW4    |
|        |        I7       |       |     SW3    |
|        |        I8       |       |     SW2    |
|        |        I9       |       |     SW1    |