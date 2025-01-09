CC=clang
CFLAGS=-std=c23 -Wall

debug: CFLAGS += -g
debug: CFLAGS += -DDEBUG
debug: tsk

release: CFLAGS += -O3 -g
release: tsk

prebuild:
	mkdir -p build/
	mkdir -p build/utils

## Main Node Files

tsk: prebuild node instruction tsk_loader utils src/tsk.c 
	$(CC) $(CFLAGS) -o tsk src/tsk.c build/*.o build/utils/*.o

node: src/tsk-src/node.h src/tsk-src/node.c
	$(CC) $(CFLAGS) -c src/tsk-src/node.c -o build/node.o

instruction: src/tsk-src/instruction.h src/tsk-src/instruction.c
	$(CC) $(CFLAGS) -c src/tsk-src/instruction.c -o build/instruction.o

tsk_loader: src/tsk-src/tsk_loader.h src/tsk-src/tsk_loader.c
	$(CC) $(CFLAGS) -c src/tsk-src/tsk_loader.c -o build/tsk_loader.o

## Utilities

utils: linked_list

linked_list: src/tsk-src/utils/linkedlist.c src/tsk-src/utils/linkedlist.h
	$(CC) $(CFLAGS) -c src/tsk-src/utils/linkedlist.c -o build/utils/linkedlist.o

clean:
	rm -rf **.log
	rm -rf build
	rm -f tsk