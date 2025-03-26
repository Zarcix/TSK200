#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/tsk/node.h"
#include "../../src/tsk/tsk_constants.h"

void node_init_with_no_instructions() {
    Node *toTest = malloc(sizeof(Node));
    Instruction *instList = NULL;
    node_init(toTest, instList);

    // Check Instruction Initialization
    assert(toTest->instructionList == instList);
    assert(toTest->instructionCount == 0);
    assert(toTest->instructionPointer == 0);

    // Check Register Initialization
    assert(toTest->ACC == 0);
    assert(toTest->BAK == 0);
    assert(toTest->LAST == NIL);

    // Check Port Initialization
    for (int i = 0; i < PIPE_COUNT; i++) {
        assert(toTest->connectedPipes[i] == NULL);
    }

    free(toTest);
    printf("PASS - node_init_with_no_instructions\n");
}

void node_init_runner() {
    node_init_with_no_instructions();
}