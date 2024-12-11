CC=gcc
CFLAGS=-std=c23 -pedantic -Wall -pedantic-errors -Wundef -Wstrict-prototypes  -Wmissing-declarations

all: tsk

tsk: src/tsk.c
	$(CC) $(CFLAGS) -o tsk src/tsk.c

clean:
	rm tsk