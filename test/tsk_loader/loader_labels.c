#include <criterion/criterion.h>

#include "../test_statics.h"

#include "../../src/tsk/node.h"
#include "../../src/tsk_misc/tsk_loader.h"

Test(TSK_LOADER_TESTS, LABEL, .init=setup_node, .fini=teardown_node) {
    tsksrc_to_node(toTest, "./test/test_files/tsk_loader/jump");
    
    OPCode expected_opcodes[] = {
        LABEL,
        JMP,
        JGZ,
        LABEL,
        JLZ,
        JEZ,
        LABEL,
        JNZ,
        JRO,
        JRO
    };

    Data expected_src[] = {
        {
            .type=STRING,
            .value.dataStr="L3"
        },
        {
            .type=STRING,
            .value.dataStr="L3"
        },
        {
            .type=STRING,
            .value.dataStr="L3"
        },
        {
            .type=STRING,
            .value.dataStr="L2"
        },
        {
            .type=STRING,
            .value.dataStr="L2"
        },
        {
            .type=STRING,
            .value.dataStr="L2"
        },
        {
            .type=STRING,
            .value.dataStr="L1"
        },
        {
            .type=STRING,
            .value.dataStr="L1"
        },
        {
            .type=PORT,
            .value.dataPort=ACC
        },
        {
            .type=NUMBER,
            .value.dataVal=5
        },
    };

    // General Tests
    cr_assert(toTest->instructionCount == 10);

    // Check Hashmap for entries
    int l1Loc = *(int*)hashmap_get(&toTest->labelMap, "L1", strlen("L1"));
    int l2Loc = *(int*)hashmap_get(&toTest->labelMap, "L2", strlen("L2"));
    int l3Loc = *(int*)hashmap_get(&toTest->labelMap, "L3", strlen("L3"));
    cr_expect_eq(l1Loc, 6);
    cr_expect_eq(l2Loc, 3);
    cr_expect_eq(l3Loc, 0);

    for (int i = 0; i < toTest->instructionCount; i++) {
        // Ensure opcodes match
        cr_expect_eq(toTest->instructionList[i].operation, expected_opcodes[i]);

        // Ensure src values match. No dest values are expected since all jump instructions have only one arg
        cr_expect_eq(toTest->instructionList[i].src.type, expected_src[i].type);

        switch (expected_src[i].type) {
            case PORT: {
                cr_expect_eq(toTest->instructionList[i].src.value.dataPort, expected_src[i].value.dataPort);
                break;
            }
            case NUMBER: {
                cr_expect_eq(toTest->instructionList[i].src.value.dataVal, expected_src[i].value.dataVal);
                break;
            }
            case STRING: {
                cr_expect_str_eq(toTest->instructionList[i].src.value.dataStr, expected_src[i].value.dataStr);
                break;
            }
        }
    }
}