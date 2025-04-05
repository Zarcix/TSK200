#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
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
    Node *left = malloc(sizeof(Node));
    Node *right = malloc(sizeof(Node));
    Node *down = malloc(sizeof(Node));
    node_init(left);
    node_init(right);
    node_init(down);

    tsksrc_to_node(left, "left");
    tsksrc_to_node(right, "right");
    tsksrc_to_node(down, "down");
    
    Pipe topPipe = {.data=NULL}, bottomPipe = {.data=NULL}, vertLeftPipe = {.data=NULL}, vertRightPipe = {.data=NULL};
    sem_init(&topPipe.dataLock, 0, 1);
    sem_init(&bottomPipe.dataLock, 0, 1);
    sem_init(&vertLeftPipe.dataLock, 0, 1);
    sem_init(&vertRightPipe.dataLock, 0, 1);

    left->writePipes[RIGHT] = &topPipe;
    left->readPipes[RIGHT] = &bottomPipe;
    right->readPipes[LEFT] = &topPipe;
    right->writePipes[LEFT] = &bottomPipe;

    left->writePipes[DOWN] = &vertRightPipe;
    left->readPipes[DOWN] = &vertLeftPipe;
    down->readPipes[UP] = &vertRightPipe;
    down->writePipes[UP] = &vertLeftPipe;
    

    threadArgs leftArg = {
        .node=left,
        .name="Left",
    };

    threadArgs rightArg = {
        .node=right,
        .name="Right",
    };
    
    threadArgs downArg = {
        .node=down,
        .name="Down",
    };

    thrd_t leftThrd, rightThrd, downThrd;

    thrd_create(&leftThrd, run_node, &leftArg);
    thrd_create(&rightThrd, run_node, &rightArg);
    thrd_create(&downThrd, run_node, &downArg);

    thrd_join(leftThrd, NULL);
    thrd_join(rightThrd, NULL);
    thrd_join(downThrd, NULL);

    return 0;
}
