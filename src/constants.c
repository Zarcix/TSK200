#include "./constants.h"

struct hashmap_s NODE_MAPS;
bool NODE_OUTPUT = false;

void init_constants() {
    hashmap_create(32, &NODE_MAPS);
}
