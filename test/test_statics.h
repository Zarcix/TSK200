#ifndef TSK_NODE_TEST
#define TSK_NODE_TEST

#include "../src/tsk/node.h"

static Node* toTest;

static void setup_node() {
    toTest = (Node*) malloc(sizeof(Node));
    node_init(toTest);
}

static void teardown_node() {
    node_cleanup(toTest);
    free(toTest);
}

typedef struct {
    Node* node;
    char name[MAX_STR_SIZE];
} threadArgs;

static int run_node(void* arg) {
    threadArgs *args = (threadArgs*)arg;
    Node* node = args->node;
    char* name = args->name;

    while (true) {
        node_tick(node);
    }

    return 0;
}


#endif
