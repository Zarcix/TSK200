#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <threads.h>
#include <time.h>

#include "./constants.h"
#include "./args.h"

#include "./tsk/node.h"
#include "./tsk_misc/tsk_loader.h"
#include "utils/hashmap.h"

thrd_t *nodeThreads = NULL;

typedef struct {
    Node* node;
    char* name;
} threadArgs;

int run_node(void* arg) {
    threadArgs *args = (threadArgs*)arg;
    Node* node = args->node;
    char* name = args->name;
    free(arg);

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
    if (NODE_OUTPUT) printf("Node '%s' registered.\n", nodeName);
    tsksrc_to_node(node, nodeName);
    return 0;
}

int link_nodes(void* const context, struct hashmap_element_s* const e) {
    Node* node = (Node*) e->data;
    char* nodeName = (char*) e->key;
    
    tsktopo_link_node(&NODE_MAPS, node, nodeName);
    return 0;
}

int start_nodes(void* const context, struct hashmap_element_s* const e) {
    Node* node = (Node*) e->data;
    char* nodeName = (char*) e->key;

    int* index = (int*) context;

    threadArgs* nodeArgs = malloc(sizeof(threadArgs));
    nodeArgs->node = node;
    nodeArgs->name = nodeName;

    thrd_create(&nodeThreads[*index], run_node, nodeArgs);

    *index += 1;
    return 0;
}

int cleanup_nodes(void* const context, struct hashmap_element_s* const e) {
    Node* node = (Node*) e->data;
    char* nodeName = (char*) e->key;

    node_cleanup(node);
    free(node);
    free(nodeName);
    return 0;
}

int main(int argc, char **argv) {
    init_constants();
    parse_args(argc, argv);

    // Parse Nodes
    hashmap_iterate_pairs(&NODE_MAPS, init_nodes, NULL);
    hashmap_iterate_pairs(&NODE_MAPS, link_nodes, NULL);

    // Start Nodes
    unsigned int nodeCount = NODE_MAPS.size;
    nodeThreads = calloc(sizeof(thrd_t), nodeCount);

    int index = 0;
    hashmap_iterate_pairs(&NODE_MAPS, start_nodes,&index);

    for (int i = 0; i < nodeCount; i++) {
        thrd_join(nodeThreads[i], NULL);
    }

    hashmap_iterate_pairs(&NODE_MAPS, cleanup_nodes, NULL);
    hashmap_destroy(&NODE_MAPS);

    free(nodeThreads);
    return 0;
}
