CFLAGS = `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -std=c90
LDLIBS =  `sdl2-config --libs` -lm
CC = gcc

# Ensure sure this file is called 'Makefile' or 'makefile'
# and then type: make blocks, make clean or make run

blocks: blocks.c neillsdl2.c neillsdl2.h
	$(CC) blocks.c neillsdl2.c -o blocks $(CFLAGS) $(LDLIBS)

clean:
	rm -f blocks

run: blocks
	./blocks
