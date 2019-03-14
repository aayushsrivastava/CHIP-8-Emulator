#include <iostream>
#include <SDL2/SDL.h>
#include "display.h"
#include "keyboard.h"
#include "chip8.h"

int main(int argc, char * argv[]) {
	Display display(960, 480);
	Keyboard keyboard;
	chip8 cpu;

	display.initScreen();
	display.clearScreen();
	Uint32 lastRenderTime = SDL_GetTicks();

	cpu.initialize();
	cpu.loadGame(argv[1]);
//	cpu.loadGame("./roms/BRIX");
	
	bool quit = false;
	while (!quit) {

		if (cpu.program_terminated) {
			keyboard.waitForQuit();
			break;
		}

		keyboard.checkInput(quit, cpu.key_states);

		cpu.emulateCycle();
		cpu.updateTimer();

		//update screen at 60Hz
		if (cpu.draw_flag) {
			Uint32 presentTime = SDL_GetTicks();
			if ((presentTime - lastRenderTime) < 17) 
				SDL_Delay(17 - (presentTime - lastRenderTime));
			
			lastRenderTime = SDL_GetTicks();
			display.updateScreen(cpu.screen_state);
			cpu.draw_flag = false;
		}

//		SDL_Delay(2);
	}
	
	std::cout << "Outside loop\n";
	display.destroyScreen();
}
