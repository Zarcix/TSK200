#include <criterion/criterion.h>
#include <criterion/assert.h>

#include "../test_statics.h"

#include "../../src/tsk/node.h"

Test(NODE_FUNCTIONALITY, SWP, .init=setup_node, .fini=teardown_node) {
    { // ADD 1
        Instruction inst = {
            .operation=ADD,

            .src.type=NUMBER,
            .src.value.dataVal=1
        };
        toTest->instructionList[0] = inst;
    }
    { // SWP
        Instruction inst = {
            .operation=SWP,
        };
        toTest->instructionList[1] = inst;
    }

    toTest->instructionCount = 2;

    node_tick(toTest);
    node_tick(toTest);

    cr_expect_eq(toTest->ACC, 0);
    cr_expect_eq(toTest->BAK, 1);
}

Test(NODE_FUNCTIONALITY, SAV, .init=setup_node, .fini=teardown_node) {
    { // ADD 1
        Instruction inst = {
            .operation=ADD,

            .src.type=NUMBER,
            .src.value.dataVal=1
        };
        toTest->instructionList[0] = inst;
    }
    { // SWP
        Instruction inst = {
            .operation=SAV,
        };
        toTest->instructionList[1] = inst;
    }

    toTest->instructionCount = 2;

    node_tick(toTest);
    node_tick(toTest);

    cr_expect_eq(toTest->ACC, 1);
    cr_expect_eq(toTest->BAK, 1);
}