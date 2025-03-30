#include <criterion/criterion.h>
#include <criterion/assert.h>

#include "../../src/tsk/node.h"

static Node *toTest = NULL;

void node_jump_setup() {
    toTest = malloc(sizeof(Node));
}

void node_jump_teardown() {

}