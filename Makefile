OBJS = ./src/display.cpp ./src/emulator.cpp ./src/keyboard.cpp ./src/chip8.cpp
CC = g++
COMPILER_FLAGS = -w
LINKER_FLAGS = -lSDL2
OBJ_NAME = chip8-emulator

for all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
