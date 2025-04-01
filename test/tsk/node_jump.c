#include <criterion/criterion.h>
#include <criterion/assert.h>
#include <stdio.h>

#include "../test_statics.h"

#include "../../src/tsk/node.h"

Test(NODE_JUMP, JMP, .init=setup_node, .fini=teardown_node) {
    // Create label
    Instruction label = {
        .operation=LABEL,
        .src.type = STRING,
        .src.value.dataStr = strdup("LABESL"),
    };

    char* mapkey = strdup(label.src.value.dataStr);
    int* mapval = malloc(sizeof(int));
    *mapval = 0;
    hashmap_put(&toTest->labelMap, mapkey, strlen(mapkey), mapval);

    // ADD -2
    Instruction add_neg_2 = {
        .operation=ADD,
        .src.type = NUMBER,
        .src.value.dataVal = -2,
    };

    Instruction add_1 = {
        .operation=SUB,
        .src.type = NUMBER,
        .src.value.dataVal = -1,
    };

    Instruction jmp = {
        .operation=JMP,
        .src.type = STRING,
        .src.value.dataStr = strdup("LABESL"),
    };

    toTest->instructionList[0] = label;
    toTest->instructionList[1] = add_neg_2;
    toTest->instructionList[2] = add_1;
    toTest->instructionList[3] = jmp;
    toTest->instructionCount = 4;

    // Go past label
    node_tick(toTest);

    // Do math until after the jump
    node_tick(toTest);
    node_tick(toTest);
    node_tick(toTest);

    // Do math one more time and jump again
    node_tick(toTest);
    node_tick(toTest);
    node_tick(toTest);

    cr_expect_eq(toTest->ACC, -2);
    cr_expect_eq(toTest->instructionPointer, 1);
}

// Test(NODE_JUMP, JEZ, .init=setup_node, .fini=teardown_node) {

// }

// Test(NODE_JUMP, JNZ, .init=setup_node, .fini=teardown_node) {

// }


// Test(NODE_JUMP, JGZ, .init=setup_node, .fini=teardown_node) {
    
// }


// Test(NODE_JUMP, JLZ, .init=setup_node, .fini=teardown_node) {
    
// }


// Test(NODE_JUMP, JRO, .init=setup_node, .fini=teardown_node) {
    
// }