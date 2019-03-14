class chip8 {
private:
	/* The opcodes are defined with two bytes
	 * The main memory is 4KB in size with word legth of one byte. 
	 */
	unsigned short opcode; 
	unsigned char memory[4096]; 
	
	/* 15 8-bit general purpose registers and one carry flag
	 * Index register and program counter need to store 12-bit addresses to the
	 * memory
	 */
	unsigned char V[16];
	unsigned short I;
	unsigned short PC;

	/* Must count down to 0 at 60Hz
	 */
	unsigned char delay_timer;
	unsigned char sound_timer;
	Uint32 timeout;

	/* The location of current instruction must be stored before calling a
	 * subroutine. This can be accomplished by making use of a stack structure
	 * and stack pointer
	 */ 
	unsigned short stack[16];
	unsigned short sp;

	unsigned char hexadecimal_sprite[16][5] = {
		{0xF0, 0x90, 0x90, 0x90, 0xF0}, //0
		{0x20, 0x60, 0x20, 0x20, 0x70}, //1
		{0xF0, 0x10, 0xF0, 0x80, 0xF0}, //2
		{0xF0, 0x10, 0xF0, 0x10, 0xF0}, //3
		{0x90, 0x90, 0xF0, 0x10, 0x10}, //4
		{0xF0, 0x80, 0xF0, 0x10, 0xF0}, //5
		{0xF0, 0x80, 0xF0, 0x90, 0xF0}, //6
		{0xF0, 0x10, 0x20, 0x40, 0x40}, //7
		{0xF0, 0x90, 0xF0, 0x90, 0xF0}, //8
		{0xF0, 0x90, 0xF0, 0x10, 0xF0}, //9
		{0xF0, 0x90, 0xF0, 0x90, 0x90}, //A
		{0xE0, 0x90, 0xE0, 0x90, 0xE0}, //B
		{0xF0, 0x80, 0x80, 0x80, 0xF0}, //C
		{0xE0, 0x90, 0x90, 0x90, 0xE0}, //D
		{0xF0, 0x80, 0xF0, 0x80, 0xF0}, //E
		{0xF0, 0x80, 0xF0, 0x80, 0x80}  //F
	};

	/* The resolution of the screen is 64 * 32 pixels.
	 * Each pixel has only two states or colors: black or white.
	 * The two colors can be represented through 0 or 1.
	 */
	unsigned char gfx[32][64];

	const unsigned int BG_COLOR = 0x33333300;
	const unsigned int DR_COLOR = 0x8F875F00;

public:
	unsigned int screen_state[64 * 32];
	bool draw_flag;
	bool program_terminated;

	/* Hex based keypad (0 to F)
	 * Array is used to store the current state of the key
	 */
	unsigned char key_states[16];

	void initialize();
	void loadGame(std::string);
	void emulateCycle();
	void updateTimer();
	void setKeys();

};

