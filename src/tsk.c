#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <argp.h>
#include <time.h>
#include "./tsk/node.h"
#include "./tsk_misc/tsk_loader.h"

const int PARSABLE_ARGS = 3;

static char* COMMAND_PATH = "";

static int NODE_MAX_OUTPUTS = 0;
static int TICK_DELAY = 0;
int milliseconds = 250;

int main(int argc, char **argv) {
    Node *test = malloc(sizeof(Node));
    node_init(test);
    
    tsksrc_to_node(test, "test");

    Pipe *left = malloc(sizeof(Pipe));
    Pipe *right = malloc(sizeof(Pipe));
    Pipe *up = malloc(sizeof(Pipe));
    Pipe *down = malloc(sizeof(Pipe));

    test->connectedPipes[LEFT] = left;
    test->connectedPipes[RIGHT] = right;
    test->connectedPipes[UP] = up;
    test->connectedPipes[DOWN] = down;

    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;

    while (1) {
        node_tick(test);
        printf("Res: %d\n", test->ACC);
        nanosleep(&ts, NULL);
    }
}
