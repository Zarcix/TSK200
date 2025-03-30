### Constants ###

CC=clang
CFLAGS=-std=c23 -Wall

TSKSRC=src/tsk
TSKBLD=build/tsk

UTILSRC=src/utils
UTILBLD=build/utils

MSCSRC=src/tsk_misc
MSCBLD=build/tsk_misc

## Main Run Commands

test: prebuild misc utils instruction node 
	$(CC) $(CFLAGS) -o test_runner test/runner.c $(MSCBLD)/*.o $(TSKBLD)/*.o $(UTILBLD)/*.o -l criterion
	./test_runner

debug: CFLAGS += -g
debug: CFLAGS += -DDEBUG
debug: tsk

release: CFLAGS += -O3 -g
release: tsk

## Sub Commands

# TSK
node: $(TSKSRC)/node.h $(TSKSRC)/node.c
	$(CC) $(CFLAGS) -c $(TSKSRC)/node.c -o $(TSKBLD)/node.o

instruction: $(TSKSRC)/instruction.h $(TSKSRC)/instruction.c
	$(CC) $(CFLAGS) -c $(TSKSRC)/instruction.c -o $(TSKBLD)/instruction.o

# UTILS

utils: linked_list strfun

linked_list: $(UTILSRC)/linkedlist.h $(UTILSRC)/linkedlist.c
	$(CC) $(CFLAGS) -c $(UTILSRC)/linkedlist.c -o $(UTILBLD)/linkedlist.o

strfun: $(UTILSRC)/strfun.h $(UTILSRC)/strfun.c
	$(CC) $(CFLAGS) -c $(UTILSRC)/strfun.c -o $(UTILBLD)/strfun.o

# MISC

misc: tsk_loader

tsk_loader: $(MSCSRC)/tsk_loader.h $(MSCSRC)/tsk_loader.c
	$(CC) $(CFLAGS) -c $(MSCSRC)/tsk_loader.c -o $(MSCBLD)/tsk_loader.o

## Main TSK

prebuild:
	mkdir -p $(MSCBLD)
	mkdir -p $(TSKBLD)
	mkdir -p $(UTILBLD)

tsk: prebuild utils node instruction misc src/tsk.c 
	$(CC) $(CFLAGS) -o tsk src/tsk.c $(MSCBLD)/*.o $(TSKBLD)/*.o $(UTILBLD)/*.o

## Post Processing

clean:
	rm -rf **.log
	rm -rf build
	rm -f tsk