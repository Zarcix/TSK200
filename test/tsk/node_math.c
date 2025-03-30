#include <criterion/criterion.h>
#include <criterion/assert.h>
#include <stdbool.h>

#include "../test_statics.h"

#include "../../src/tsk/node.h"

Test(NODE_MATH, NOP_FUNCTIONALITY, .init=setup_node, .fini=teardown_node) {
    Instruction toExecute;
    toExecute.operation = NOP;

    node_execute_instruction(toTest, toExecute);

    cr_expect(toTest->ACC == 0);
    cr_expect(toTest->BAK == 0);
}

Test(NODE_MATH, ADD_FUNCTIONALITY, .init=setup_node, .fini=teardown_node) {
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

    cr_expect(toTest->ACC == -6);

}

Test(NODE_MATH, SUB_FUNCTIONALITY, .init=setup_node, .fini=teardown_node) {
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

    cr_expect(toTest->ACC == 6);
}

Test(NODE_MATH, NEG_FUNCTIONALITY, .init=setup_node, .fini=teardown_node) {
    toTest->ACC = 15;
    Instruction negExec;
    negExec.operation = NEG;

    node_execute_instruction(toTest, negExec);
    cr_expect(toTest->ACC == -15);
}