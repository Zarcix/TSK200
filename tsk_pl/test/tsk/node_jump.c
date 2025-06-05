#include <criterion/criterion.h>
#include <criterion/assert.h>

#include "../test_statics.h"

#include "../../src/tsk/node.h"

Test(NODE_FUNCTIONALITY, JMP, .init=setup_node, .fini=teardown_node) {
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

Test(NODE_FUNCTIONALITY, JEZ, .init=setup_node, .fini=teardown_node) {
    char* labelName = strdup("TOP");
    { // MOV -1 ACC
        Instruction inst = {
            .operation=MOV,

            .src.type=NUMBER,
            .src.value.dataVal=-1,

            .dest.type=PORT,
            .dest.value.dataPort=ACC
        };
        toTest->instructionList[0] = inst;
    }
    { // TOP:
        int* labelLoc = malloc(sizeof(int));
        *labelLoc = 1;
        Instruction inst = {
            .operation=LABEL,
            .src.type=STRING,
            .src.value.dataStr=labelName,
        };
        hashmap_put(&toTest->labelMap, labelName, strlen(labelName), labelLoc);

        toTest->instructionList[1] = inst;
    }
    { // ADD 1
        Instruction inst = {
            .operation=ADD,

            .src.type=NUMBER,
            .src.value.dataVal=1,
        };
        toTest->instructionList[2] = inst;
    }
    { // JEZ TOP
        Instruction inst = {
            .operation=JEZ,

            .src.type=STRING,
            .src.value.dataStr=strdup("TOP"),
        };
        toTest->instructionList[3] = inst;
    }
    toTest->instructionCount = 4;

    // If this is not added properly, the rest will not work
    cr_assert_eq(*(int*)hashmap_get(&toTest->labelMap, labelName, strlen(labelName)), 1);

    for (int _ = 0; _ < 20; _++) {
        node_tick(toTest);
    }
    free(labelName);

    cr_expect_eq(toTest->ACC, -1);
    cr_expect_eq(toTest->instructionPointer, 2);
}

Test(NODE_FUNCTIONALITY, JNZ, .init=setup_node, .fini=teardown_node) {
    char* labelName = strdup("TOP");
    { // MOV -5 ACC
        Instruction inst = {
            .operation=MOV,

            .src.type=NUMBER,
            .src.value.dataVal=-5,

            .dest.type=PORT,
            .dest.value.dataPort=ACC
        };
        toTest->instructionList[0] = inst;
    }
    { // TOP:
        int* labelLoc = malloc(sizeof(int));
        *labelLoc = 1;
        Instruction inst = {
            .operation=LABEL,
            .src.type=STRING,
            .src.value.dataStr=labelName,
        };
        hashmap_put(&toTest->labelMap, labelName, strlen(labelName), labelLoc);

        toTest->instructionList[1] = inst;
    }
    { // ADD 1
        Instruction inst = {
            .operation=ADD,

            .src.type=NUMBER,
            .src.value.dataVal=1,
        };
        toTest->instructionList[2] = inst;
    }
    { // JNZ TOP
        Instruction inst = {
            .operation=JNZ,

            .src.type=STRING,
            .src.value.dataStr=strdup("TOP"),
        };
        toTest->instructionList[3] = inst;
    }

    toTest->instructionCount = 4;

    // If this is not added properly, the rest will not work
    cr_assert_eq(*(int*)hashmap_get(&toTest->labelMap, labelName, strlen(labelName)), 1);

    for (int _ = 0; _ < 15; _++) {
        node_tick(toTest);
    }
    free(labelName);

    cr_expect_eq(toTest->ACC, -4);
    cr_expect_eq(toTest->instructionPointer, 3);
}

Test(NODE_FUNCTIONALITY, JGZ, .init=setup_node, .fini=teardown_node) {
    char* labelName = strdup("TOP");
    { // MOV 5 ACC
        Instruction inst = {
            .operation=MOV,

            .src.type=NUMBER,
            .src.value.dataVal=5,

            .dest.type=PORT,
            .dest.value.dataPort=ACC
        };
        toTest->instructionList[0] = inst;
    }
    { // TOP:
        int* labelLoc = malloc(sizeof(int));
        *labelLoc = 1;
        Instruction inst = {
            .operation=LABEL,
            .src.type=STRING,
            .src.value.dataStr=labelName,
        };
        hashmap_put(&toTest->labelMap, labelName, strlen(labelName), labelLoc);

        toTest->instructionList[1] = inst;
    }
    { // SUB 1
        Instruction inst = {
            .operation=SUB,

            .src.type=NUMBER,
            .src.value.dataVal=1,
        };
        toTest->instructionList[2] = inst;
    }
    { // JGZ TOP
        Instruction inst = {
            .operation=JGZ,

            .src.type=STRING,
            .src.value.dataStr=strdup("TOP"),
        };
        toTest->instructionList[3] = inst;
    }

    toTest->instructionCount = 4;

    // If this is not added properly, the rest will not work
    cr_assert_eq(*(int*)hashmap_get(&toTest->labelMap, labelName, strlen(labelName)), 1);

    for (int _ = 0; _ < 15; _++) {
        node_tick(toTest);
    }
    free(labelName);

    cr_expect_eq(toTest->ACC, 4);
    cr_expect_eq(toTest->instructionPointer, 3);
}

Test(NODE_FUNCTIONALITY, JLZ, .init=setup_node, .fini=teardown_node) {
    char* labelName = strdup("TOP");
    { // MOV -5 ACC
        Instruction inst = {
            .operation=MOV,

            .src.type=NUMBER,
            .src.value.dataVal=-5,

            .dest.type=PORT,
            .dest.value.dataPort=ACC
        };
        toTest->instructionList[0] = inst;
    }
    { // TOP:
        int* labelLoc = malloc(sizeof(int));
        *labelLoc = 1;
        Instruction inst = {
            .operation=LABEL,
            .src.type=STRING,
            .src.value.dataStr=labelName,
        };
        hashmap_put(&toTest->labelMap, labelName, strlen(labelName), labelLoc);

        toTest->instructionList[1] = inst;
    }
    { // ADD 1
        Instruction inst = {
            .operation=ADD,

            .src.type=NUMBER,
            .src.value.dataVal=1,
        };
        toTest->instructionList[2] = inst;
    }
    { // JLZ TOP
        Instruction inst = {
            .operation=JLZ,

            .src.type=STRING,
            .src.value.dataStr=strdup("TOP"),
        };
        toTest->instructionList[3] = inst;
    }

    toTest->instructionCount = 4;

    // If this is not added properly, the rest will not work
    cr_assert_eq(*(int*)hashmap_get(&toTest->labelMap, labelName, strlen(labelName)), 1);

    for (int _ = 0; _ < 15; _++) {
        node_tick(toTest);
    }
    free(labelName);

    cr_expect_eq(toTest->ACC, -4);
    cr_expect_eq(toTest->instructionPointer, 3);
}

Test(NODE_FUNCTIONALITY, JRO, .init=setup_node, .fini=teardown_node) {
    char* labelName = strdup("TOP");
    { // ADD 2
        Instruction inst = {
            .operation=ADD,

            .src.type=NUMBER,
            .src.value.dataVal=2,
        };
        toTest->instructionList[0] = inst;
    }
    { // TOP:
        int* labelLoc = malloc(sizeof(int));
        *labelLoc = 1;
        Instruction inst = {
            .operation=LABEL,
            .src.type=STRING,
            .src.value.dataStr=labelName,
        };
        hashmap_put(&toTest->labelMap, labelName, strlen(labelName), labelLoc);

        toTest->instructionList[1] = inst;
    }
    { // JRO ACC
        Instruction inst = {
            .operation=JRO,

            .src.type=PORT,
            .src.value.dataPort=ACC,
        };
        toTest->instructionList[2] = inst;
    }
    { // ADD 1
        Instruction inst = {
            .operation=ADD,

            .src.type=NUMBER,
            .src.value.dataVal=1,
        };
        toTest->instructionList[3] = inst;
    }
    { // NEG
        Instruction inst = {
            .operation=NEG,
        };
        toTest->instructionList[4] = inst;
    }
    { // JRO ACC
        Instruction inst = {
            .operation=JRO,

            .src.type=PORT,
            .src.value.dataPort=ACC,
        };
        toTest->instructionList[5] = inst;
    }
    { // NEG
        Instruction inst = {
            .operation=NEG,
        };
        toTest->instructionList[6] = inst;
    }
    { // JMP T
        Instruction inst = {
            .operation=JMP,

            .src.type=STRING,
            .src.value.dataStr=labelName,
        };
        toTest->instructionList[7] = inst;
    }

    toTest->instructionCount = 8;

    for (int _ = 0; _ < 25; _++) {
        node_tick(toTest);
    }

    cr_expect_eq(toTest->instructionPointer, 2);
    cr_expect_eq(toTest->ACC, 1);

}

Test(NODE_FUNCTIONALITY, JRO_OUT_OF_BOUNDS_BEG, .init=setup_node, .fini=teardown_node) {
    { // ADD 2
        Instruction inst = {
            .operation=ADD,

            .src.type=NUMBER,
            .src.value.dataVal=2,
        };
        toTest->instructionList[0] = inst;
    }
    { // JRO -5
        Instruction inst = {
            .operation=JRO,

            .src.type=NUMBER,
            .src.value.dataVal=-5,
        };
        toTest->instructionList[1] = inst;
    }

    toTest->instructionCount = 2;

    for (int _ = 0; _ < 8; _++) {
        node_tick(toTest);
    }

    cr_expect_eq(toTest->ACC, 8);
    cr_expect_eq(toTest->instructionPointer, 0);
}

Test(NODE_FUNCTIONALITY, JRO_OUT_OF_BOUNDS_END, .init=setup_node, .fini=teardown_node) {
    { // ADD 2
        Instruction inst = {
            .operation=ADD,

            .src.type=NUMBER,
            .src.value.dataVal=2,
        };
        toTest->instructionList[0] = inst;
    }
    { // JRO -1
        Instruction inst = {
            .operation=JRO,

            .src.type=NUMBER,
            .src.value.dataVal=5,
        };
        toTest->instructionList[1] = inst;
    }

    toTest->instructionCount = 2;

    for (int _ = 0; _ < 5; _++) {
        node_tick(toTest);
    }

    cr_expect_eq(toTest->ACC, 2);
    cr_expect_eq(toTest->instructionPointer, 1);
}

Test(NODE_FUNCTIONALITY, JRO_JUMP_BACK_LABEL, .init=setup_node, .fini=teardown_node) {
    char* labelName = strdup("TOP");
    { // ADD 2
        Instruction inst = {
            .operation=ADD,

            .src.type=NUMBER,
            .src.value.dataVal=2,
        };
        toTest->instructionList[0] = inst;
    }
    { // TOP:
        int* labelLoc = malloc(sizeof(int));
        *labelLoc = 1;
        Instruction inst = {
            .operation=LABEL,
            .src.type=STRING,
            .src.value.dataStr=labelName,
        };
        hashmap_put(&toTest->labelMap, labelName, strlen(labelName), labelLoc);

        toTest->instructionList[1] = inst;
    }
    { // JRO -5
        Instruction inst = {
            .operation=JRO,

            .src.type=NUMBER,
            .src.value.dataVal=-1,
        };
        toTest->instructionList[2] = inst;
    }

    toTest->instructionCount = 3;

    // If this is not added properly, the rest will not work
    cr_assert_eq(*(int*)hashmap_get(&toTest->labelMap, labelName, strlen(labelName)), 1);

    for (int _ = 0; _ < 10; _++) {
        node_tick(toTest);
    }
    free(labelName);
    cr_expect_eq(toTest->ACC, 8);
    cr_expect_eq(toTest->instructionPointer, 1);
}

Test(NODE_FUNCTIONALITY, JRO_JUMP_FORWARD_LABEL, .init=setup_node, .fini=teardown_node) {
    char* labelName = strdup("TOP");
    { // JRO 1
        Instruction inst = {
            .operation=JRO,

            .src.type=NUMBER,
            .src.value.dataVal=1,
        };
        toTest->instructionList[0] = inst;
    }
    { // TOP:
        int* labelLoc = malloc(sizeof(int));
        *labelLoc = 1;
        Instruction inst = {
            .operation=LABEL,
            .src.type=STRING,
            .src.value.dataStr=labelName,
        };
        hashmap_put(&toTest->labelMap, labelName, strlen(labelName), labelLoc);

        toTest->instructionList[1] = inst;
    }
    { // ADD 5
        Instruction inst = {
            .operation=ADD,

            .src.type=NUMBER,
            .src.value.dataVal=5,
        };
        toTest->instructionList[2] = inst;
    }
    { // NEG
        Instruction inst = {
            .operation=NEG,
        };
        toTest->instructionList[3] = inst;
    }
    { // JRO ACC
        Instruction inst = {
            .operation=JRO,

            .src.type=PORT,
            .src.value.dataPort=ACC,
        };
        toTest->instructionList[4] = inst;
    }

    toTest->instructionCount = 5;

    // If this is not added properly, the rest will not work
    cr_assert_eq(*(int*)hashmap_get(&toTest->labelMap, labelName, strlen(labelName)), 1);

    for (int _ = 0; _ < 15; _++) {
        node_tick(toTest);
    }
    free(labelName);
    cr_expect_eq(toTest->ACC, 0);
    cr_expect_eq(toTest->instructionPointer, 4);
}
