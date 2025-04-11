#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <threads.h>
#include <time.h>

#include "./constants.h"
#include "./args.c"

#include "./tsk/node.h"
#include "./tsk_misc/tsk_loader.h"
#include "utils/hashmap.h"

typedef struct {
    Node* node;
    char* name;
} threadArgs;

int run_node(void* arg) {
    threadArgs *args = (threadArgs*)arg;
    Node* node = args->node;
    char* name = args->name;

    while (true) {
        if (NODE_OUTPUT) {
            printf("Node '%s' | IP: %d | ACC: %d\n", name, node->instructionPointer, node->ACC);
        }
        node_tick(node);
    }

    return 0;
}

int init_nodes(void* const context, struct hashmap_element_s* const e) {
    Node* node = (Node*) e->data;
    char* nodeName = (char*) e->key;
    tsksrc_to_node(node, nodeName);
    return 0;
}

int link_nodes(void* const context, struct hashmap_element_s* const e) {
    return 0;
}

int main(int argc, char **argv) {
    parse_args(argc, argv);

    struct hashmap_element_s* const temp = NULL;
    hashmap_iterate_pairs(&NODE_MAPS, init_nodes, temp);
    hashmap_iterate_pairs(&NODE_MAPS, link_nodes, temp);

    return 0;
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
