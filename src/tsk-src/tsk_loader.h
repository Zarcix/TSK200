#ifndef TSK_LOADER
#define TSK_LOADER

#include "./node.h"
#include <stdlib.h>

typedef struct {
    char *name;
    Node* node;
} NodeMapIndex;

typedef struct {
    int node_count;
    Node** node_list;
} NodeList;

static NodeMapIndex* node_mappings = NULL;
static int node_mapping_count = 0;

NodeList* tsk_to_node(char* tskPath);
char* tsk_get_node_name(Node* nodeToSearch);
void tsk_save_output(Node* outNode);
#endif
