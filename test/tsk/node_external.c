#include <criterion/criterion.h>
#include <criterion/assert.h>
#include <threads.h>
#include <signal.h>

#include "../test_statics.h"

#include "../../src/tsk/node.h"

Test(NODE_FUNCTIONALITY, WRITE_ACC, .timeout=5) {
}

Test(NODE_FUNCTIONALITY, WRITE_NIL, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, WRITE_LAST, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, WRITE_ANY, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, WRITE_LEFT, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, WRITE_RIGHT, .timeout=5) {
    // Init Nodes
    Node node1;
    Node node2;
    node_init(&node1);
    node_init(&node2);

    Pipe topPipe = {.data=NULL}, bottomPipe = {.data=NULL};
    sem_init(&topPipe.dataLock, 0, 1);
    sem_init(&bottomPipe.dataLock, 0, 1);

    node1.writePipes[RIGHT] = &topPipe;
    node2.readPipes[LEFT] = &topPipe;

    node2.writePipes[LEFT] = &bottomPipe;
    node1.readPipes[RIGHT] = &bottomPipe;

    // Init Instructions
    { // Node 1
        { // MOV 1 RIGHT
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=1,

                .dest.type=PORT,
                .dest.value.dataPort=RIGHT,
            };
            node1.instructionList[0] = inst;
        }
        { // MOV 5 RIGHT
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=5,

                .dest.type=PORT,
                .dest.value.dataPort=RIGHT,
            };
            node1.instructionList[1] = inst;
        }
        { // EXT
            Instruction inst = {
                .operation=EXT,
            };
            node1.instructionList[2] = inst;
        }
        node1.instructionCount = 3;
    }
    { // Node 2
        { // ADD LEFT
            Instruction inst = {
                .operation=ADD,

                .src.type=PORT,
                .src.value.dataVal=LEFT,
            };
            node2.instructionList[0] = inst;
        }
        { // ADD LEFT
            Instruction inst = {
                .operation=ADD,

                .src.type=PORT,
                .src.value.dataVal=LEFT,
            };
            node2.instructionList[1] = inst;
        }
        { // EXT
            Instruction inst = {
                .operation=EXT,
            };
            node2.instructionList[2] = inst;
        }
        node2.instructionCount = 3;
    }

    // Start threads for nodes
    pthread_t node1Thrd, node2Thrd;

    threadArgs node1Args = {
        .node=&node1,
        .name="Node 1",
    };

    threadArgs node2Args = {
        .node=&node2,
        .name="Node 2",
    };

    thrd_create(&node1Thrd, run_node, &node1Args);
    thrd_create(&node2Thrd, run_node, &node2Args);

    int res = 0;
    thrd_join(node1Thrd, &res);
    cr_expect_eq(res, SIGTERM);
    thrd_join(node2Thrd, &res);
    cr_expect_eq(res, SIGTERM);

    // Check all nodes
    cr_expect_eq(node1.instructionPointer, 2);
    cr_expect_eq(node2.instructionPointer, 2);
    cr_expect_eq(node2.ACC, 6);
}

Test(NODE_FUNCTIONALITY, WRITE_UP, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, WRITE_DOWN, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, READ_ACC, .timeout=5) {

}

Test(NODE_FUNCTIONALITY, READ_NIL, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, READ_LAST, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, READ_ANY, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, READ_LEFT, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, READ_RIGHT, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, READ_UP, .timeout=5) {
    
}

Test(NODE_FUNCTIONALITY, READ_DOWN, .timeout=5) {
    
}
