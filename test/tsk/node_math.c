#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/tsk/node.h"

Node* node_math_generate_node() {
    Node *toTest = malloc(sizeof(Node));
    node_init(toTest, NULL);
    return toTest;
}

void node_math_nop() {
    Node* toTest = node_math_generate_node();

    Instruction toExecute;
    toExecute.operation = NOP;

    node_execute_instruction(toTest, toExecute);

    assert(toTest->ACC == 0);
    assert(toTest->BAK == 0);

    free(toTest);
    printf("PASS - node_math_nop\n");
}

void node_math_add() {
    Node* toTest = node_math_generate_node();

    Instruction posNumExec;
    posNumExec.operation = ADD;

    posNumExec.src.type = NUMBER;
    posNumExec.src.value.dataVal = 5;


    Instruction zeroNumExec;
    zeroNumExec.operation = ADD;

    zeroNumExec.src.type = NUMBER;
    zeroNumExec.src.value.dataVal = 0;

    Instruction negNumExec;
    negNumExec.operation = ADD;

    negNumExec.src.type = NUMBER;
    negNumExec.src.value.dataVal = -8;

    Instruction regValExec;
    regValExec.operation = ADD;
    regValExec.src.type = PORT;
    regValExec.src.value.dataPort = ACC;

    node_execute_instruction(toTest, posNumExec);
    node_execute_instruction(toTest, zeroNumExec);
    node_execute_instruction(toTest, negNumExec);
    node_execute_instruction(toTest, regValExec);

    assert(toTest->ACC == -6);

    free(toTest);

    printf("PASS - node_math_add\n");
}

void node_math_sub() {
    Node* toTest = node_math_generate_node();

    Instruction posNumExec;
    posNumExec.operation = SUB;

    posNumExec.src.type = NUMBER;
    posNumExec.src.value.dataVal = 5;


    Instruction zeroNumExec;
    zeroNumExec.operation = SUB;

    zeroNumExec.src.type = NUMBER;
    zeroNumExec.src.value.dataVal = 0;

    Instruction negNumExec;
    negNumExec.operation = SUB;

    negNumExec.src.type = NUMBER;
    negNumExec.src.value.dataVal = -8;

    Instruction regValExec;
    regValExec.operation = ADD;
    regValExec.src.type = PORT;
    regValExec.src.value.dataPort = ACC;

    node_execute_instruction(toTest, posNumExec);
    node_execute_instruction(toTest, zeroNumExec);
    node_execute_instruction(toTest, negNumExec);
    node_execute_instruction(toTest, regValExec);

    assert(toTest->ACC == 6);

    free(toTest);

    printf("PASS - node_math_sub\n");
}

void node_math_neg() {
    Node* toTest = node_math_generate_node();
    toTest->ACC = 15;

    Instruction negExec;
    negExec.operation = NEG;

    node_execute_instruction(toTest, negExec);

    assert(toTest->ACC == -15);

    free(toTest);
    printf("PASS - node_math_neg\n");
}

void node_math_runner() {
    node_math_nop();
    node_math_add();
    node_math_sub();
    node_math_neg();
}