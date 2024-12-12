CC=gcc
CFLAGS=-std=c23 -pedantic -Wall -pedantic-errors -Wundef -Wstrict-prototypes  -Wmissing-declarations -g

all: tsk

prebuild:
	mkdir -p build/

tsk: prebuild src/tsk.c node opcodes
	$(CC) $(CFLAGS) -o tsk src/tsk.c build/node.o build/opcodes.o

node: src/tsk-src/node.h src/tsk-src/node.c
	$(CC) $(CFLAGS) -c src/tsk-src/node.c -o build/node.o

opcodes: src/tsk-src/opcodes.h src/tsk-src/opcodes.c
	$(CC) $(CFLAGS) -c src/tsk-src/opcodes.c -o build/opcodes.o

clean:
	rm -rf build
	rm -f tsk