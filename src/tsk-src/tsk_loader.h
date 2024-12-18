#include "./node.h"

typedef struct {
    char *name;
    Node* node;
} NodeMapIndex;

Node** tsk_to_node(char* tskPath);
