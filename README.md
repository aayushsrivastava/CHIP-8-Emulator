# CHIP-8-Emulator
An interpretative emulator for CHIP-8 written in C++ and SDL2

As of now, the emulator is able to completely run a good number of CHIP-8 programs. There's still a bug somewhere in the opcode execution (apparently) which is causing some programs to not run properly. The implementation of display, keyboard and timers are complete and there are no issues with them as far as I know. Sound hasn't been implemented yet.

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
I haven't tried compiling it on Windows. But there isn't any platform-dependent code in there so it shouldn't be causing any problems afaik. You'll probably want to look up how to setup SDL2's development libraries on Visual Studio or Mingw32.

## Run the program
```
$ ./chip8-emulator [path to rom file]
```

## Controls
The CHIP-8 consists of a hex keyboard (read the [References](https://github.com/aayushsrivastava/CHIP-8-Emulator#references) for more info). They have been mapped to the following keys: 
```
     1 2 3 C             1 2 3 4
     4 5 6 D     --->     Q W E R
     7 8 9 E     --->      A S D F
     A 0 B F                Z X C V
(CHIP-8 Keyboard)     (Mapped Keyboard)
```

## ROMs
You can easily find a lot of public domain CHIP-8 programs/games on the internet. 

## Reading the source code
* **chip8.h** contains the implementation of CPU and timers.
* **display.h** wraps the SDL2 procedures to create and maintain a screen instance.
* **keyboard.h** maps the original Chip-8 key states to the qwerty keyboard. It also keeps track of when user wants to quit the program.
* Graphics, keymapping and timing, all make use of SDL2 to maintain platform independence.

## References
* [CHIP-8 - Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
* [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
* [Simple DirectMedia Layer (SDL2) Wiki](https://wiki.libsdl.org/CategoryAPI)
