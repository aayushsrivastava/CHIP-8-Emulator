#include <iostream>
#include "display.h"

Display::Display(int width = 640, int height = 320) {
	window = nullptr;
	renderer = nullptr;
	texture = nullptr;

	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;

	CHIP8_WIDTH = 64;
	CHIP8_HEIGHT = 32;
}

void Display::logSDL_Error(std::string msg) {
	std::cout << msg << " Error: " << SDL_GetError();
}

bool Display::initScreen() {
	bool initSuccessful = true;
	window = SDL_CreateWindow("CHIP8 Emulator",
		       	SDL_WINDOWPOS_UNDEFINED,
		       	SDL_WINDOWPOS_UNDEFINED,
		       	SCREEN_WIDTH, 
			SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		logSDL_Error("CreateWindow");
		initSuccessful = false;
	}
	else {
		/* The rendering is displayed on 'window'
		 * -1 to initialize the first rendering driver supporting the requested flags
		 * Hardware acceleration is used
		 */
		renderer = SDL_CreateRenderer(window, 
				-1, 
				SDL_RENDERER_ACCELERATED);

		if (renderer == nullptr) {
			logSDL_Error("CreateRenderer");
			SDL_DestroyWindow(window);
			window = nullptr; //You didn't pass reference to window ;)
			initSuccessful = false;
		}
		else {
			/* The original 64x32 screen will be upscaled to the
			 * target resolution which is the resolution of the 
			 * renderer
			 */
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");	
			texture = SDL_CreateTexture(renderer, 
					SDL_PIXELFORMAT_RGBA8888, 
					SDL_TEXTUREACCESS_STREAMING, 
					CHIP8_WIDTH, 
					CHIP8_HEIGHT);
			if (texture == nullptr) {
				logSDL_Error("CreateTexture");
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				initSuccessful = false;
			}
		}
	}
	return initSuccessful;
}

void Display::clearScreen() {
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void Display::updateScreen(Uint32* pixels) {
	int pitch = CHIP8_WIDTH * sizeof(Uint32);
	SDL_UpdateTexture(texture, nullptr, pixels, pitch);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);	
}

void Display::destroyScreen() {
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	texture = nullptr;
	renderer = nullptr;
	window = nullptr;
}
