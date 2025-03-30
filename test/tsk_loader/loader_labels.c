#include <criterion/criterion.h>
#include <stdio.h>

#include "../../src/tsk/node.h"
#include "../../src/tsk_misc/tsk_loader.h"

static Node *loader_label_toTest = NULL;

void loader_label_setup() {
    loader_label_toTest = malloc(sizeof(Node));
    node_init(loader_label_toTest);
}

void loader_label_teardown() {
    node_cleanup(loader_label_toTest);
    free(loader_label_toTest);
}

Test(TSK_LOADER_TESTS, LABEL, .init=loader_label_setup, .fini=loader_label_teardown) {
    tsksrc_to_node(loader_label_toTest, "./test/test_files/tsk_loader/jump");
    
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
    cr_assert(loader_label_toTest->instructionCount == 10);

    // Check Hashmap for entries
    int l1Loc = *(int*)hashmap_get(&loader_label_toTest->labelMap, "L1", strlen("L1"));
    int l2Loc = *(int*)hashmap_get(&loader_label_toTest->labelMap, "L2", strlen("L2"));
    int l3Loc = *(int*)hashmap_get(&loader_label_toTest->labelMap, "L3", strlen("L3"));
    cr_expect_eq(l1Loc, 6);
    cr_expect_eq(l2Loc, 3);
    cr_expect_eq(l3Loc, 0);

    for (int i = 0; i < loader_label_toTest->instructionCount; i++) {
        // Ensure opcodes match
        cr_expect_eq(loader_label_toTest->instructionList[i].operation, expected_opcodes[i]);

        // Ensure src values match. No dest values are expected since all jump instructions have only one arg
        cr_expect_eq(loader_label_toTest->instructionList[i].src.type, expected_src[i].type);

        switch (expected_src[i].type) {
            case PORT: {
                cr_expect_eq(loader_label_toTest->instructionList[i].src.value.dataPort, expected_src[i].value.dataPort);
                break;
            }
            case NUMBER: {
                cr_expect_eq(loader_label_toTest->instructionList[i].src.value.dataVal, expected_src[i].value.dataVal);
                break;
            }
            case STRING: {
                cr_expect_str_eq(loader_label_toTest->instructionList[i].src.value.dataStr, expected_src[i].value.dataStr);
                break;
            }
        }
    }
}