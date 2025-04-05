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

#endif
