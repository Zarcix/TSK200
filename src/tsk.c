#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <threads.h>
#include <time.h>

#include "./tsk/node.h"
#include "./tsk_misc/tsk_loader.h"

const int PARSABLE_ARGS = 3;

static char* COMMAND_PATH = "";

static int NODE_MAX_OUTPUTS = 0;
static int TICK_DELAY = 0;
int milliseconds = 250;

typedef struct {
    Node* node;
    char name[MAX_STR_SIZE];
} threadArgs;

int run_node(void* arg) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;

    threadArgs *args = (threadArgs*)arg;
    Node* node = args->node;
    char* name = args->name;
    int exitVal = 0;

    while (true) {
        printf("Node '%s' | IP: %d | ACC: %d\n", name, node->instructionPointer, node->ACC);
        exitVal = node->ACC;
        node_tick(node);
        nanosleep(&ts, NULL);
    }

    return 0;
}

int main(int argc, char **argv) {
    Node left;
    Node right;
    node_init(&left);
    node_init(&right);

    tsksrc_to_node(&left, "left");
    tsksrc_to_node(&right, "right");
    
    Pipe *topPipe = malloc(sizeof(Pipe));
    topPipe->data = NULL;

    Pipe *bottomPipe = malloc(sizeof(Pipe));
    topPipe->data = NULL;

    sem_init(&topPipe->dataLock, 0, 1);
    sem_init(&bottomPipe->dataLock, 0, 1);

    left.writePipes[RIGHT] = topPipe;
    left.readPipes[RIGHT] = bottomPipe;
    right.readPipes[LEFT] = topPipe;
    right.writePipes[LEFT] = bottomPipe;

    threadArgs leftArg = {
        .node=&left,
        .name="Left",
    };

    threadArgs rightArg = {
        .node=&right,
        .name="Right",
    };

    thrd_t leftThrd, rightThrd;

    thrd_create(&leftThrd, run_node, &leftArg);
    thrd_create(&rightThrd, run_node, &rightArg);

    thrd_join(leftThrd, NULL);
    thrd_join(rightThrd, NULL);

    node_cleanup(&left);
    node_cleanup(&right);

    free(topPipe);
    free(bottomPipe);

    return 0;
}
