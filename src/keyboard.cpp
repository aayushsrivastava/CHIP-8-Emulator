#include "keyboard.h"

void Keyboard::checkInput(bool &quit, unsigned char keys[]) {
	while(SDL_PollEvent(&e) != 0) {
		//User wants to quit
		if (e.type == SDL_QUIT) {
			quit = true;
		}
		//User presses a key
		else if(e.type == SDL_KEYDOWN) {
			switch(e.key.keysym.sym) {

			case SDLK_1:
			keys[MAPPED_KEY_1] = 1;	
			break;

			case SDLK_2:
			keys[MAPPED_KEY_2] = 1;
			break;

			case SDLK_3:
			keys[MAPPED_KEY_3] = 1;
			break;

			case SDLK_4:
			keys[MAPPED_KEY_4] = 1;
			break;

			case SDLK_q:
			keys[MAPPED_KEY_Q] = 1;
			break;

			case SDLK_w:
			keys[MAPPED_KEY_W] = 1;
			break;

			case SDLK_e:
			keys[MAPPED_KEY_E] = 1;
			break;
			
			case SDLK_r:
			keys[MAPPED_KEY_R] = 1;
			break;

			case SDLK_a:
			keys[MAPPED_KEY_A] = 1;
			break;

			case SDLK_s:
			keys[MAPPED_KEY_S] = 1;
			break;

			case SDLK_d:
			keys[MAPPED_KEY_D] = 1;
			break;

			case SDLK_f:
			keys[MAPPED_KEY_F] = 1;
			break;

			case SDLK_z:
			keys[MAPPED_KEY_Z] = 1;
			break;

			case SDLK_x:
			keys[MAPPED_KEY_X] = 1;
			break;

			case SDLK_c:
			keys[MAPPED_KEY_C] = 1;
			break;

			case SDLK_v:
			keys[MAPPED_KEY_V] = 1;
			break;

			}
		}
		//User releases a key
		else if(e.type == SDL_KEYUP) {
			switch(e.key.keysym.sym) {

			case SDLK_1:
			keys[MAPPED_KEY_1] = 0;	
			break;

			case SDLK_2:
			keys[MAPPED_KEY_2] = 0;
			break;

			case SDLK_3:
			keys[MAPPED_KEY_3] = 0;
			break;

			case SDLK_4:
			keys[MAPPED_KEY_4] = 0;
			break;

			case SDLK_q:
			keys[MAPPED_KEY_Q] = 0;
			break;

			case SDLK_w:
			keys[MAPPED_KEY_W] = 0;
			break;

			case SDLK_e:
			keys[MAPPED_KEY_E] = 0;
			break;
			
			case SDLK_r:
			keys[MAPPED_KEY_R] = 0;
			break;

			case SDLK_a:
			keys[MAPPED_KEY_A] = 0;
			break;

			case SDLK_s:
			keys[MAPPED_KEY_S] = 0;
			break;

			case SDLK_d:
			keys[MAPPED_KEY_D] = 0;
			break;

			case SDLK_f:
			keys[MAPPED_KEY_F] = 0;
			break;

			case SDLK_z:
			keys[MAPPED_KEY_Z] = 0;
			break;

			case SDLK_x:
			keys[MAPPED_KEY_X] = 0;
			break;

			case SDLK_c:
			keys[MAPPED_KEY_C] = 0;
			break;

			case SDLK_v:
			keys[MAPPED_KEY_V] = 0;
			break;

			}
		}

	}
}

void Keyboard::waitForQuit() {
	while(1) {
		SDL_WaitEvent(&e);
		if (e.type == SDL_QUIT)
			return;
	}
}
