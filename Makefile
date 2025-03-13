CC=clang
CFLAGS=-std=c23 -Wall

debug: CFLAGS += -g
debug: CFLAGS += -DDEBUG
debug: tsk

release: CFLAGS += -O3 -g
release: tsk

## Node Components

TSKSRC=src/tsk
TSKBLD=build/tsk

node: $(TSKSRC)/node.h $(TSKSRC)/node.c
	$(CC) $(CFLAGS) -c $(TSKSRC)/node.c -o $(TSKBLD)/node.o

instruction: $(TSKSRC)/instruction.h $(TSKSRC)/instruction.c
	$(CC) $(CFLAGS) -c $(TSKSRC)/instruction.c -o $(TSKBLD)/instruction.o

tsk_loader: $(TSKSRC)/tsk_loader.h $(TSKSRC)/tsk_loader.c
	$(CC) $(CFLAGS) -c $(TSKSRC)/tsk_loader.c -o $(TSKBLD)/tsk_loader.o

## Extra Utilities

UTILSRC=src/utils
UTILBLD=build/utils

utils: linked_list

linked_list: $(UTILSRC)/linkedlist.c $(UTILSRC)/linkedlist.h
	$(CC) $(CFLAGS) -c $(UTILSRC)/linkedlist.c -o $(UTILBLD)/linkedlist.o

## Main TSK

prebuild:
	mkdir -p $(TSKBLD)
	mkdir -p $(UTILBLD)

tsk: prebuild node instruction tsk_loader utils src/tsk.c 
	$(CC) $(CFLAGS) -o tsk src/tsk.c build/*.o build/utils/*.o

## Post Processing

clean:
	rm -rf **.log
	rm -rf build
	rm -f tsk