#ifndef CONSTANTS
#define CONSTANTS
#include <stdbool.h>

#include "./utils/hashmap.h"

static struct hashmap_s NODE_MAPS;
static bool NODE_OUTPUT = false;

static void init_constants() {
    hashmap_create(32, &NODE_MAPS);
}

#endif
