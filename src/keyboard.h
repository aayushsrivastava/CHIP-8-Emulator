#include <SDL2/SDL.h>

typedef enum {
	MAPPED_KEY_X,
	MAPPED_KEY_1,
	MAPPED_KEY_2,
	MAPPED_KEY_3,
	MAPPED_KEY_Q,
	MAPPED_KEY_W,
	MAPPED_KEY_E,
	MAPPED_KEY_A,
	MAPPED_KEY_S,
	MAPPED_KEY_D,
	MAPPED_KEY_Z,
	MAPPED_KEY_C,
	MAPPED_KEY_4,
	MAPPED_KEY_R,
	MAPPED_KEY_F,
	MAPPED_KEY_V
} mapped_keys;

class Keyboard {
private:
	SDL_Event e;
public:
	void checkInput(bool &, unsigned char *);
	void waitForQuit();
};
