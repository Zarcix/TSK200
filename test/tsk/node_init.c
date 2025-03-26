#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <criterion/criterion.h>

#include "../../src/tsk/node.h"
#include "../../src/tsk/tsk_constants.h"

static Node *toTest;

void setup(void) {
    toTest = malloc(sizeof(Node));
}

void node_init_teardown(void) {
    node_cleanup(toTest);
    free(toTest);
}

Test(NODE_INIT, EMPTY_INSTRUCTION_LIST, .init=setup, .fini=node_init_teardown) {
    node_init(toTest, NULL);

    // Check Instruction Initialization
    cr_expect_null(toTest->instructionList);
    cr_expect(toTest->instructionCount == 0);
    cr_expect(toTest->instructionPointer == 0);

    // Check Register Initialization
    cr_expect(toTest->ACC == 0);
    cr_expect(toTest->BAK == 0);
    cr_expect(toTest->LAST == NIL);

    // Check Register Initialization
    cr_expect(toTest->ACC == 0);
    cr_expect(toTest->BAK == 0);
    cr_expect(toTest->LAST == NIL);

    // Check Port Initialization
    for (int i = 0; i < PIPE_COUNT; i++) {
        cr_expect_null(toTest->connectedPipes[i]);
    }
}