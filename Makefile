CC=clang
CFLAGS=-std=c23 -pedantic -Wall -pedantic-errors -g

all: tsk

prebuild:
	mkdir -p build/

tsk: prebuild src/tsk.c node instruction tsk_loader
	$(CC) $(CFLAGS) -o tsk src/tsk.c build/*

node: src/tsk-src/node.h src/tsk-src/node.c
	$(CC) $(CFLAGS) -c src/tsk-src/node.c -o build/node.o

instruction: src/tsk-src/instruction.h src/tsk-src/instruction.c
	$(CC) $(CFLAGS) -c src/tsk-src/instruction.c -o build/instruction.o

tsk_loader: src/tsk-src/tsk_loader.h src/tsk-src/tsk_loader.c
	$(CC) $(CFLAGS) -c src/tsk-src/tsk_loader.c -o build/tsk_loader.o

clean:
	rm -rf build
	rm -f tsk