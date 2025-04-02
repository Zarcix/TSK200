#include <criterion/criterion.h>

#include "../test_statics.h"

#include "../../src/tsk/node.h"
#include "../../src/tsk_misc/tsk_loader.h"

Test(TSK_LOADER, MATH, .init=setup_node, .fini=teardown_node) {
    tsksrc_to_node(toTest, "./test/test_files/tsk_loader/math");
    OPCode expected_opcodes[] = {
        ADD,
        ADD,
        ADD,
        ADD,
        ADD,
        SUB,
        SUB,
        SUB,
        SUB,
        NEG,
        NOP
    };

    Data expected_src[] = {
        {
            .type=NUMBER,
            .value.dataVal=5
        },
        {
            .type=PORT,
            .value.dataPort=RIGHT
        },
        {
            .type=PORT,
            .value.dataPort=LEFT
        },
        {
            .type=PORT,
            .value.dataPort=ANY
        },
        {
            .type=PORT,
            .value.dataPort=NIL
        },
        {
            .type=PORT,
            .value.dataPort=UP
        },
        {
            .type=PORT,
            .value.dataPort=DOWN
        },
        {
            .type=PORT,
            .value.dataPort=ACC
        },
        {
            .type=PORT,
            .value.dataPort=LAST
        },
        {
            .type=PORT,
            .value.dataPort=NIL
        },
        {
            .type=PORT,
            .value.dataPort=NIL
        },
    };

    // General Tests
    cr_assert(toTest->instructionCount == 11);

    for (int i = 0; i < toTest->instructionCount; i++) {
        // Ensure opcodes match
        cr_assert_eq(toTest->instructionList[i].operation, expected_opcodes[i]);

        // Ensure src values match. No dest values are expected since all math instructions have only one arg
        cr_assert_eq(toTest->instructionList[i].src.type, expected_src[i].type);

        switch (expected_src[i].type) {
            case PORT: {
                cr_assert_eq(toTest->instructionList[i].src.value.dataPort, expected_src[i].value.dataPort);
                break;
            }
            case NUMBER: {
                cr_assert_eq(toTest->instructionList[i].src.value.dataVal, expected_src[i].value.dataVal);
                break;
            }
            case STRING: {
                cr_assert_str_eq(toTest->instructionList[i].src.value.dataStr, expected_src[i].value.dataStr);
                break;
            }
        }
    }
}