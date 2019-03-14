# CHIP-8-Emulator
An interpretative emulator for CHIP-8 written in C++ and SDL2

The emulator is able to fully run a good number of CHIP-8 programs. The implementation of display, keyboard and timers are complete and there are no issues with them as far as I know. There's still a bug somewhere in the opcode execution apparently which is causing some programs to not run properly. Sound hasn't been implemented yet.

## Installation
To be able to use SDL2
```
$ sudo apt install libsdl2-dev
```
Clone this repo
```
$ git clone https://github.com/aayushsrivastava/CHIP-8-Emulator.git
$ cd CHIP-8-Emulator
```
Compile
```
$ make all
```
I haven't tested on Windows but there isn't any platform dependent code in there so it shouldn't cause any problem.

## Run the program
```
$ ./chip8-emulator [path to rom file]
```

## Controls
The CHIP-8 consists of a hex keyboard (read the [References](https://github.com/aayushsrivastava/CHIP-8-Emulator/new/master?readme=1#references) for more info). They have been mapped to the following keys: 
```
1 2 3 4
Q W E R
A S D F
Z X C V
```

## ROMs
You can easily find a lot of public domain CHIP-8 programs/games on the internet. 

## References
* [CHIP-8 - Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
* [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
