CC=gcc
CFLAGS=-std=c23 -pedantic -Wall -pedantic-errors -Wundef -Wstrict-prototypes  -Wmissing-declarations -g

all: tsk

prebuild:
	mkdir -p build/

tsk: prebuild src/tsk.c node instruction
	$(CC) $(CFLAGS) -o tsk src/tsk.c build/node.o build/instruction.o

node: src/tsk-src/node.h src/tsk-src/node.c
	$(CC) $(CFLAGS) -c src/tsk-src/node.c -o build/node.o

instruction: src/tsk-src/instruction.h src/tsk-src/instruction.c
	$(CC) $(CFLAGS) -c src/tsk-src/instruction.c -o build/instruction.o

clean:
	rm -rf build
	rm -f tsk