#ifndef TSK_LOADER
#define TSK_LOADER

#include "./node.h"
#include <stdlib.h>

typedef struct {
    char *name;
    Node* node;
} NodeMapIndex;

static NodeMapIndex* node_mappings = NULL;
static int node_mapping_count = 0;

Node** tsk_to_node(char* tskPath);
char* tsk_node_name(Node* nodeToSearch);
#endif
