#include <iostream>
#include <random>
#include <fstream>
#include <SDL2/SDL.h>
#include "chip8.h"

/* TODO
 * Check PC updation after each instruction
 * check OS and include necessary libraries (not required)
 * test compilation on windows
 */

void chip8::initialize() {
	//Clearing memories and registers
	for (int i = 0; i < 4096; i++) 
		memory[i] = 0;
	for (int i = 0; i < 16; i++) 
		V[i] = 0;

	I = 0;
	PC = 0x200; //ROM is loaded from memory location 512 onwards
	

	//Clearing Stack
	sp = 0;
	stack[sp] = 0;

	//Clearing display
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 64; j++) {
			gfx[i][j] = 0;
		}
	}	

	//Clearing internal flags
	draw_flag = false;
	program_terminated = false;

	//Load fontset in memory
	for (int i = 0; i < 80; i++) 
		memory[i] = (unsigned char) *(((char *) hexadecimal_sprite) + i);

	//Reseting key press states
	for (int i = 0; i < 16; i++)
		key_states[i] = 0;

	//Reset timers
	delay_timer = 0;
	sound_timer = 0;

}

void chip8::loadGame(std::string gameName) {
	/* The program (ROM file) is copied from specified address into the memory
	 */
	std::ifstream gameBinary;
	gameBinary.open(gameName.c_str(), std::ifstream::binary);

	gameBinary.seekg(0, gameBinary.end);
	int numberofBytes = gameBinary.tellg();
	gameBinary.seekg(0, gameBinary.beg);

	gameBinary.read((char *) memory + 512, numberofBytes);
	gameBinary.close();
}

void chip8::emulateCycle() {
	/* The general cycle of CPU
	 * 1. Fetch opcode
	 * 2. Decode opcode
	 * 3. Execute opcode
	 * 4. Update timers if necessary
	 */


	// Fetch opcode
	opcode = memory[PC] << 8 | memory[PC + 1];
	std::cout << std::hex << opcode << ' ' << PC << std::endl;

	// Decode opcode 
	switch(opcode & 0xF000) {
	
	case 0x0000: {
	/* OPCODE: 0???
	 * Three different opcodes with above pattern
	 */
		switch(opcode & 0x000F) {
		/* OPCODE: 00E0
		 * Clear the screen
		 */
		case 0x0000: {
			for (int i = 0; i < 32; i++) {
				for (int j = 0; j < 64; j++) {
					gfx[i][j] = 0;
				}
			}
			PC += 2;
		} break;
			     
		/* OPCODE: 00EE
		 * Return from a subroutine
		 */
		case 0x000E: {
			sp -= 1;
			PC = stack[sp];
			PC += 2;
		}
		break;

		default: 
		/* OPCODE 0NNN
		 * Jump to machine routine at NNN
		 * Not needed on modern interpreters
		 */
			PC += 2;
		}
	} break;

	/* OPCODE: 1NNN
	 * Jump to address NNN
	 */
	case 0x1000: {
		if ((opcode & 0x0FFF) == PC)
			program_terminated = true;
		PC = opcode & 0x0FFF;
	} break;
	
	/* OPCODE: 2NNN
	 * Call subroutine at NNN
	 */
	case 0x2000: {
		stack[sp] = PC;
		sp += 1;
		PC = opcode & 0x0FFF;
	} break;

	/* OPCODE: 3XNN
	 * Skip the next instruction if VX equals NN
	 */
	case 0x3000: {
		if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
			PC += 4;
		}
		else {
			PC += 2;
		}	
	} break;

	/* OPCODE: 4XNN
	 * Skip the next instruction if VX != NN
	 */
	case 0x4000: {
 		if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
			PC += 4;
		}
		else {
			PC += 2;
		}
	} break;

	/* OPCODE: 5XY0
	 * Skip the next instruction if VX == VY
	 */
	case 0x5000: {
		if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
			PC += 4;
		}
		else {
			PC += 2;
		}
	} break;

	/* OPCODE: 6XNN
	 * Put the value NN into the register VX
	 */
	case 0x6000: {
		V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
		PC += 2;
	} break;
	
	/* OPCODE: 7XNN
	 * Add the value NN to the value of VX and store the result in register
	 * VX. Ignore the carry.
	 */
	case 0x7000: {
		V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
		PC += 2;
	} break;
		 
	case 0x8000: {
	/* OPCODE: 8XY?
	 * Nine different instructions with above pattern
	 */
		unsigned char X = (opcode & 0x0F00) >> 8;
		unsigned char Y = (opcode & 0x00F0) >> 4;

		switch(opcode & 0x000F) {

		/* OPCODE: 8XY0
		 * Store the value of VY in VX
		 */
		case 0x0000: {
			V[X] = V[Y];
			PC += 2;
		} break;	

		/* OPCODE: 8XY1
		 * Perform a bitwise OR operation between the values of VX and 
		 * VY and store the result in VX
		 */
		case 0x0001: {
			V[X] = V[X] | V[Y];
			PC += 2;
		} break;	

		/* OPCODE: 8XY2
		 * Perform a bitwise AND operation between the values of VX and
		 * VY and store the result in VX
		 */
		case 0x0002: {
			V[X] = V[X] & V[Y];
			PC += 2;
		} break;

		/* OPCODE: 8XY3
		 * Perform a bitwise XOR operation between the values of VX and
		 * VY and store the result in VX
		 */
		case 0x0003: {
			V[X] = V[X] ^ V[Y];
			PC += 2;
		} break;
		
		/* OPCODE: 8XY4
		 * Add the values of VX and VY and store in VX
		 * VF is set to 1 if there's a carry and to 0 if there isn't
		 */
		case 0x0004: {
			V[15] = (V[X] > (0xFF - V[Y]));
			V[X] += V[Y];
			PC += 2;
		} break;

		/* OPCODE: 8XY5
		 * Subtract value of VY from VX and store the result in VX
		 * VF is set to 0 if there's a borrow and to 1 if there isn't
		 * ie VF = NOT borrow
		 */
		case 0x0005: {
			V[15] = V[X] > V[Y];
			V[X] -= V[Y];
			PC += 2;
		} break;

		/* OPCODE: 8XY6
		 * Right shift VX
		 * Store the LSB in VF
		 */
		case 0x0006: {
			V[15] = V[X] & 0x01;
			V[X] >>= 1;
			PC += 2;
		} break;

		/* OPCODE: 8XY7
		 * Subtract value of VX from VY and store the result in VX
		 * VF = not borrow
		 */
		case 0x0007: {
			V[15] = V[Y] > V[X];
			V[X] = V[Y] - V[X];
			PC += 2;
		} break;

		/* OPCODE: 8XYE
		 * Left-shift VX
		 * Store the MSB in VF
		 */
		case 0x000E: {
			V[15] = V[X] & 0x80;
			V[X] <<= 1;
			PC += 2;
		} break;

		} //end of switch for 8XY?
	} break;

	/* OPCODE: 9XY0
	 * Skip the next instruction if VX != VY
	 */
	case 0x9000: {
		if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
			PC += 4;
		}
		else {
			PC += 2;
		}
	} break;

	/* OPCODE: ANNN
	 * Set the value of register I to NNN
	 */
	case 0xA000: {
		I = opcode & 0x0FFF;
		PC += 2;
	} break;

	/* OPCODE: BNNN
	 * Jump to location NNN + V0
	 */
	case 0xB000: {
		PC = V[0] + (opcode & 0x0FFF);
	} break;
		     
	/* OPCODE: CXNN
	 * Perform bitwise AND operation on NN with an 8bit random number
	 * Store the result in VX
	 */
	case 0xC000: {
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> dist256(0, 255);
		char randomNumber = dist256(rng);
		
		V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF) & randomNumber;
		PC += 2;
	} break;

	/* OPCODE: DXYN
	 * Draw sprite
	 */
	case 0xD000: {
		unsigned char X = V[(opcode & 0x0F00) >> 8];
		unsigned char Y = V[(opcode & 0x00F0) >> 4];
		V[0xF] = 0;
		for (unsigned char i = 0x0; i < (opcode & 0x000F); i += 0x1) {
			for (unsigned char j = 0x0; j < 0x8; j += 0x1) {
				unsigned char pixel_value = (memory[I + i] >> (7 - j)) & 0x01;
				int EX = (X + j) % 64;
				int EY = (Y + i) % 32;
				if (gfx[EY][EX] ^ pixel_value == 0) {
					V[0xF] = 1;
					draw_flag = true;
				}
				gfx[EY][EX] ^= pixel_value;
			}
		}
		PC += 2;
	} break;

	case 0xE000: {
	/* OPCODE: EX??
	 * Two different instructions with above pattern
	 */
		switch (opcode & 0x00FF) {
		/* OPCODE: EX9E
		 * If the key stored in VX is pressed, skip the next instruction
		 */
		case 0x009E: {
			if (key_states[V[(opcode & 0x0F00) >> 8]] == 1) {
				PC += 4;
			}
			else {
				PC += 2;
			}
		} break;
		
		/* OPCODE: EXA1
		 * If the key stored in VX isn't pressed, skip the next instruction
		 */
		case 0x00A1: {
			if (key_states[V[(opcode & 0x0F00) >> 8]] == 0) {
				PC += 4;
			}
			else {
				PC += 2;
			}
		} break;
		} //end of switch for EX??
	} break;
	
	case 0xF000: {
	/* OPCODE: FX??
	 * Nine instructions with above pattern
	 */
		switch (opcode & 0x00FF) {

		/* OPCODE: FX07
		 * The value of delay timer is placed in VX
		 */
		case 0x0007: {
			V[(opcode & 0x0F00) >> 8] = delay_timer;
			PC += 2;
		} break;
		
		/* OPCODE: FX0A
		 * Wait for a key press and store the value in VX
		 * The execution is stopped until a key is pressed
		 */
		case 0x000A: {
			char pressed_value;
			bool key_pressed = false;
			
			for (int i = 0; i < 16; i++) {
		   		if (key_states[i] == 1) {
					pressed_value = i;
					key_pressed = true;
					break;
				}
			}

			if (key_pressed) {
				V[(opcode & 0x0F00) >> 8] = pressed_value;
				PC += 2;
			}
			SDL_Delay(1);
		} break;

		/* OPCODE: FX15
		 * Delay timer is set to the value stored in VX
		 */
		case 0x0015: {
			delay_timer = V[(opcode & 0x0F00) >> 8];
			timeout = SDL_GetTicks() + 17;
			PC += 2;
		} break;

		/* OPCODE: FX18
		 * Sound timer is set to the value stored in VX
		 */
		case 0x0018: {
			sound_timer = V[(opcode & 0x0F00) >> 8];
			timeout = SDL_GetTicks() + 17;
			PC += 2;
		} break;

		/* OPCODE: FX1E
		 * Add values in I and VX
		 * Store the result in I
		 */
		case 0x001E: {
			I += V[(opcode & 0x0F00) >> 8];
			PC += 2;
		} break;

		/* OPCODE: FX29
		 * The address of sprite corresponding to a hexadecimal value given in VX is
		 * stored in register I
		 * The sprites are stored from address 0x0000 in the main memory
		 */
		case 0x0029: {
			I = 0x0000 + (5 * V[(opcode & 0x0F00) >> 8]);
			PC += 2;
		} break;

		/* OPCODE: FX33
		 * Store the BCD representation of decimal value stored in VX in memory
		 * from location I to I+2
		 * I     = Hundred's digit
		 * I + 1 = Ten's digit
		 * I + 2 = One's digit
		 * This can also be done more efficiently with Shift and Add-3 algorithm
		 */
		case 0x0033: {
			memory[I] = V[(opcode & 0x0F00 >> 8)] / 100;
			memory[I + 1] = (V[(opcode & 0x0F00) >> 8] % 100) / 10;
			memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
			PC += 2;
		} break;
		
		/* OPCODE: FX55
		 * Store the values from registers V0 to VX in the memory starting at location I
		 */
		case 0x0055: {
			for (unsigned char reg = 0x0; reg <= ((opcode & 0x0F00) >> 8); reg += 0x1) {
				memory[I + reg] = V[reg];
			}
			PC += 2;
		} break;
			
		/* OPCODE: FX65
		 * Read values for registers V0 to VX from the memory, starting at location I 
		 */
		case 0x0065: {
			for (unsigned char reg = 0x0; reg <= ((opcode & 0x0F00) >> 8); reg += 0x1) {
				V[reg] = memory[I + reg];
			}
			PC += 2;
		} break;
		
		default:	
			std::cout << "Error: Unknown opcode " << opcode << std::endl;
			std::cout << "jj\n";
			exit(1);
		} //end of switch for FX??

	} break; 

	default: 
	std::cout << "Error: Unknown opcode " << opcode << std::endl;
	} //end of entire decode switch
	
	if (draw_flag == false)
		return;
	//Update pixel values
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 64; j++) {
			if (gfx[i][j] == 0)
				screen_state[i * 64 + j] = BG_COLOR;
			else
				screen_state[i * 64 + j] = DR_COLOR;
		}
	}	
}

void chip8::updateTimer() {
	bool change_made = false;

	if (SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
		if (delay_timer != 0) {
			delay_timer--;
			change_made = true;
		}
		if (sound_timer != 0) {
			sound_timer--;
			change_made = true;
		}
		if (change_made)
			timeout = SDL_GetTicks() + 17;
	}
}
