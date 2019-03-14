#include <SDL2/SDL.h>

class Display {
private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	int CHIP8_WIDTH;
	int CHIP8_HEIGHT;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

public:
	Display(int, int);
	void logSDL_Error(std::string);
	bool initScreen();
	void clearScreen();
	void updateScreen(Uint32 *);
	void destroyScreen();
};
