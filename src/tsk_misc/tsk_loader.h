#ifndef TSK_LOAD
#define TSK_LOAD

#include <stdlib.h>

#include "../tsk/node.h"
#include "../tsk/tsk_constants.h"

void tsksrc_to_node(Node* node, char* filename);

void tsktopo_link_node(const struct hashmap_s* const nodeMap, Node* node, char* nodeName);

#endif
