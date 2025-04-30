#include <criterion/criterion.h>

#include "../test_statics.h"

#include "../../src/tsk/node.h"
#include "../../src/tsk/tsk_constants.h"


Test(NODE_FUNCTIONALITY, INIT, .init=setup_node, .fini=teardown_node) {
    node_init(toTest);

    // Check Instruction Initialization
    cr_expect_not_null(toTest->instructionList);
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
        cr_expect_null(toTest->readPipes[i]);
        cr_expect_null(toTest->writePipes[i]);
    }
}
