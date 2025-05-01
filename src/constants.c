#include "./constants.h"

bool NODE_OUTPUT = false;

void init_constants() {
    hashmap_create(32, &NODE_MAPS);
}