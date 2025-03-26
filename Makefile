### Constants ###

CC=clang
CFLAGS=-std=c23 -Wall

TSKSRC=src/tsk
TSKBLD=build/tsk

UTILSRC=src/utils
UTILBLD=build/utils

## Main Run Commands

test: prebuild node
	$(CC) $(CFLAGS) -o test_runner test/runner.c build/tsk/*.o -l criterion

debug: CFLAGS += -g
debug: CFLAGS += -DDEBUG
debug: tsk

release: CFLAGS += -O3 -g
release: tsk

## Sub Commands

node: $(TSKSRC)/node.h $(TSKSRC)/node.c
	$(CC) $(CFLAGS) -c $(TSKSRC)/node.c -o $(TSKBLD)/node.o

tsk_loader: $(TSKSRC)/tsk_loader.h $(TSKSRC)/tsk_loader.c
	$(CC) $(CFLAGS) -c $(TSKSRC)/tsk_loader.c -o $(TSKBLD)/tsk_loader.o

utils: linked_list

linked_list: $(UTILSRC)/linkedlist.c $(UTILSRC)/linkedlist.h
	$(CC) $(CFLAGS) -c $(UTILSRC)/linkedlist.c -o $(UTILBLD)/linkedlist.o

## Main TSK

prebuild:
	mkdir -p $(TSKBLD)
	mkdir -p $(UTILBLD)

tsk: prebuild node instruction tsk_loader utils src/tsk.c 
	$(CC) $(CFLAGS) -o tsk src/tsk.c build/tsk/*.o build/utils/*.o

## Post Processing

clean:
	rm -rf **.log
	rm -rf build
	rm -f tsk