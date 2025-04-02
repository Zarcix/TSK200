### Constants ###

CC=clang
CFLAGS=-std=gnu23 -Wall

TSKSRC=src/tsk
TSKBLD=build/tsk

UTILSRC=src/utils
UTILBLD=build/utils

MSCSRC=src/tsk_misc
MSCBLD=build/tsk_misc

## Main Run Commands

release: CFLAGS += -O3 -g
release: tsk

debug: CFLAGS += -g -DDEBUG
debug: tsk

test: CFLAGS += -g -DDEBUG
test: prebuild utils misc node instruction 
	$(CC) $(CFLAGS) -o test_runner test/runner.c $(MSCBLD)/*.o $(TSKBLD)/*.o $(UTILBLD)/*.o -l criterion
	./test_runner

clean:
	rm -rf **.log
	rm -rf build
	rm -f tsk test_runner

## Sub Commands

## Main TSK

tsk: prebuild utils misc node instruction src/tsk.c 
	$(CC) $(CFLAGS) -o tsk src/tsk.c $(MSCBLD)/*.o $(TSKBLD)/*.o $(UTILBLD)/*.o

prebuild:
	rm -rf build
	mkdir -p $(MSCBLD)
	mkdir -p $(TSKBLD)
	mkdir -p $(UTILBLD)

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
