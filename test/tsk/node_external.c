#include <criterion/criterion.h>
#include <criterion/assert.h>
#include <threads.h>
#include <signal.h>

#include "../test_statics.h"

#include "../../src/tsk/node.h"

Test(NODE_FUNCTIONALITY, WRITE_ACC, .init=setup_node, .fini=teardown_node, .timeout=5) {
    int movVal = 20;
    { // Node Instructions
        { // MOV 20 ACC
            Instruction inst = {
                .operation=MOV,
                
                .src.type=NUMBER,
                .src.value.dataVal=movVal,

                .dest.type=PORT,
                .dest.value.dataPort=ACC,
            };
            toTest->instructionList[0] = inst;
        }
        { // EXT
            Instruction inst = {
                .operation=EXT
            };
            toTest->instructionList[1] = inst;
        }
        toTest->instructionCount = 2;
    }
    thrd_t testThrd;

    threadArgs nodeArgs = {
        toTest,
    };

    cr_expect_neq(toTest->ACC, movVal);

    thrd_create(&testThrd, run_node, &nodeArgs);

    int res = 0;
    thrd_join(testThrd, &res);
    cr_expect_eq(res, SIGTERM);

    cr_expect_eq(toTest->ACC, movVal);
}

Test(NODE_FUNCTIONALITY, WRITE_NIL, .init=setup_node, .fini=teardown_node, .timeout=5) {
    int movVal = 20;
    { // Node Instructions
        { // MOV 20 ACC
            Instruction inst = {
                .operation=MOV,
                
                .src.type=NUMBER,
                .src.value.dataVal=movVal,

                .dest.type=PORT,
                .dest.value.dataPort=NIL,
            };
            toTest->instructionList[0] = inst;
        }
        { // EXT
            Instruction inst = {
                .operation=EXT
            };
            toTest->instructionList[1] = inst;
        }
        toTest->instructionCount = 2;
    }
    thrd_t testThrd;

    threadArgs nodeArgs = {
        toTest,
    };

    cr_expect_neq(toTest->ACC, movVal);

    thrd_create(&testThrd, run_node, &nodeArgs);

    int res = 0;
    thrd_join(testThrd, &res);
    cr_expect_eq(res, SIGTERM);

    cr_expect_neq(toTest->ACC, movVal);
}

Test(NODE_FUNCTIONALITY, WRITE_ANY, .timeout=5) {
    // Init Nodes
    Node left;
    Node right;
    node_init(&left);
    node_init(&right);

    Pipe topPipe = {.data=NULL}, bottomPipe = {.data=NULL};
    sem_init(&topPipe.dataLock, 0, 1);
    sem_init(&bottomPipe.dataLock, 0, 1);

    left.writePipes[RIGHT] = &topPipe;
    right.readPipes[LEFT] = &topPipe;

    right.writePipes[LEFT] = &bottomPipe;
    left.readPipes[RIGHT] = &bottomPipe;

    { // Left Node
        { // MOV 5 ANY
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=1,

                .dest.type=PORT,
                .dest.value.dataPort=ANY,
            };
            left.instructionList[0] = inst;
        }
        { // EXT
            Instruction inst = {
                .operation=EXT
            };
            left.instructionList[1] = inst;
        }
        left.instructionCount = 2;
    }
    { // Right Node
        { // MOV 5 ANY
            Instruction inst = {
                .operation=MOV,

                .src.type=PORT,
                .src.value.dataPort=LEFT,

                .dest.type=PORT,
                .dest.value.dataPort=ACC,
            };
            right.instructionList[0] = inst;
        }
        { // EXT
            Instruction inst = {
                .operation=EXT
            };
            right.instructionList[1] = inst;
        }
        right.instructionCount = 2;
    }

    // Start threads for nodes
    thrd_t node1Thrd, node2Thrd;

    threadArgs node1Args = {
        .node=&left,
    };

    threadArgs node2Args = {
        .node=&right,
    };

    thrd_create(&node1Thrd, run_node, &node1Args);
    thrd_create(&node2Thrd, run_node, &node2Args);

    int res = 0;
    thrd_join(node1Thrd, &res);
    cr_expect_eq(res, SIGTERM);
    thrd_join(node2Thrd, &res);
    cr_expect_eq(res, SIGTERM);

    // Check all nodes
    cr_expect_eq(left.instructionPointer, 1);
    cr_expect_eq(right.instructionPointer, 1);

    cr_expect_eq(right.ACC, 1);
    cr_expect_eq(left.LAST, RIGHT);
}

Test(NODE_FUNCTIONALITY, WRITE_LAST, .timeout=5) {
    // Init Nodes
    Node left;
    Node right;
    node_init(&left);
    node_init(&right);

    Pipe topPipe = {.data=NULL}, bottomPipe = {.data=NULL};
    sem_init(&topPipe.dataLock, 0, 1);
    sem_init(&bottomPipe.dataLock, 0, 1);

    left.writePipes[RIGHT] = &topPipe;
    right.readPipes[LEFT] = &topPipe;

    right.writePipes[LEFT] = &bottomPipe;
    left.readPipes[RIGHT] = &bottomPipe;

    { // Left Node
        { // MOV 5 ANY
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=5,

                .dest.type=PORT,
                .dest.value.dataPort=ANY,
            };
            left.instructionList[0] = inst;
        }
        { // MOV 10 LAST
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=10,

                .dest.type=PORT,
                .dest.value.dataPort=LAST,
            };
            left.instructionList[1] = inst;
        }
        { // MOV LAST ACC
            Instruction inst = {
                .operation=SUB,

                .src.type=PORT,
                .src.value.dataPort=LAST,
            };
            left.instructionList[2] = inst;
        }
        { // EXT
            Instruction inst = {
                .operation=EXT
            };
            left.instructionList[3] = inst;
        }
        left.instructionCount = 4;
    }
    { // Right Node
        { // ADD ANY
            Instruction inst = {
                .operation=ADD,

                .src.type=PORT,
                .src.value.dataPort=ANY,
            };
            right.instructionList[0] = inst;
        }
        { // SUB ANY
            Instruction inst = {
                .operation=SUB,

                .src.type=PORT,
                .src.value.dataPort=ANY,
            };
            right.instructionList[1] = inst;
        }
        { // MOV ACC LAST
            Instruction inst = {
                .operation=MOV,

                .src.type=PORT,
                .src.value.dataPort=ACC,

                .dest.type=PORT,
                .dest.value.dataPort=LAST,
            };
            right.instructionList[2] = inst;
        }
        { // EXT
            Instruction inst = {
                .operation=EXT
            };
            right.instructionList[3] = inst;
        }
        right.instructionCount = 4;
    }

    // Start threads for nodes
    thrd_t node1Thrd, node2Thrd;

    threadArgs node1Args = {
        .node=&left,
    };

    threadArgs node2Args = {
        .node=&right,
    };

    thrd_create(&node1Thrd, run_node, &node1Args);
    thrd_create(&node2Thrd, run_node, &node2Args);

    int res = 0;
    thrd_join(node1Thrd, &res);
    cr_expect_eq(res, SIGTERM);
    thrd_join(node2Thrd, &res);
    cr_expect_eq(res, SIGTERM);

    // Check IP
    cr_expect_eq(left.instructionPointer, 3);
    cr_expect_eq(right.instructionPointer, 3);

    // Check Node Vals
    cr_expect_eq(left.ACC, 5);
    cr_expect_eq(right.ACC, -5);

    // Check LAST
    cr_expect_eq(left.LAST, RIGHT);
    cr_expect_eq(right.LAST, LEFT);
}

Test(NODE_FUNCTIONALITY, WRITE_LEFT, .timeout=5) {
    // Init Nodes
    Node node1;
    Node node2;
    node_init(&node1);
    node_init(&node2);

    Pipe topPipe = {.data=NULL}, bottomPipe = {.data=NULL};
    sem_init(&topPipe.dataLock, 0, 1);
    sem_init(&bottomPipe.dataLock, 0, 1);

    node1.writePipes[LEFT] = &topPipe;
    node2.readPipes[RIGHT] = &topPipe;

    node2.writePipes[RIGHT] = &bottomPipe;
    node1.readPipes[LEFT] = &bottomPipe;

    // Init Instructions
    { // Node 1
        { // MOV 1 RIGHT
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=1,

                .dest.type=PORT,
                .dest.value.dataPort=LEFT,
            };
            node1.instructionList[0] = inst;
        }
        { // MOV 5 RIGHT
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=5,

                .dest.type=PORT,
                .dest.value.dataPort=LEFT,
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
                .src.value.dataVal=RIGHT,
            };
            node2.instructionList[0] = inst;
        }
        { // ADD LEFT
            Instruction inst = {
                .operation=ADD,

                .src.type=PORT,
                .src.value.dataVal=RIGHT,
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
    thrd_t node1Thrd, node2Thrd;

    threadArgs node1Args = {
        .node=&node1,
    };

    threadArgs node2Args = {
        .node=&node2,
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
    thrd_t node1Thrd, node2Thrd;

    threadArgs node1Args = {
        .node=&node1,
    };

    threadArgs node2Args = {
        .node=&node2,
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
    // Init Nodes
    Node node1;
    Node node2;
    node_init(&node1);
    node_init(&node2);

    Pipe topPipe = {.data=NULL}, bottomPipe = {.data=NULL};
    sem_init(&topPipe.dataLock, 0, 1);
    sem_init(&bottomPipe.dataLock, 0, 1);

    node1.writePipes[UP] = &topPipe;
    node2.readPipes[DOWN] = &topPipe;

    node2.writePipes[DOWN] = &bottomPipe;
    node1.readPipes[UP] = &bottomPipe;

    // Init Instructions
    { // Node 1
        { // MOV 1 RIGHT
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=1,

                .dest.type=PORT,
                .dest.value.dataPort=UP,
            };
            node1.instructionList[0] = inst;
        }
        { // MOV 5 RIGHT
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=5,

                .dest.type=PORT,
                .dest.value.dataPort=UP,
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
                .src.value.dataVal=DOWN,
            };
            node2.instructionList[0] = inst;
        }
        { // ADD LEFT
            Instruction inst = {
                .operation=ADD,

                .src.type=PORT,
                .src.value.dataVal=DOWN,
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
    thrd_t node1Thrd, node2Thrd;

    threadArgs node1Args = {
        .node=&node1,
    };

    threadArgs node2Args = {
        .node=&node2,
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

Test(NODE_FUNCTIONALITY, WRITE_DOWN, .timeout=5) {
    // Init Nodes
    Node node1;
    Node node2;
    node_init(&node1);
    node_init(&node2);

    Pipe topPipe = {.data=NULL}, bottomPipe = {.data=NULL};
    sem_init(&topPipe.dataLock, 0, 1);
    sem_init(&bottomPipe.dataLock, 0, 1);

    node1.writePipes[DOWN] = &topPipe;
    node2.readPipes[UP] = &topPipe;

    node2.writePipes[UP] = &bottomPipe;
    node1.readPipes[DOWN] = &bottomPipe;

    // Init Instructions
    { // Node 1
        { // MOV 1 RIGHT
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=1,

                .dest.type=PORT,
                .dest.value.dataPort=DOWN,
            };
            node1.instructionList[0] = inst;
        }
        { // MOV 5 RIGHT
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=5,

                .dest.type=PORT,
                .dest.value.dataPort=DOWN,
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
                .src.value.dataVal=UP,
            };
            node2.instructionList[0] = inst;
        }
        { // ADD LEFT
            Instruction inst = {
                .operation=ADD,

                .src.type=PORT,
                .src.value.dataVal=UP,
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
    thrd_t node1Thrd, node2Thrd;

    threadArgs node1Args = {
        .node=&node1,
    };

    threadArgs node2Args = {
        .node=&node2,
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

Test(NODE_FUNCTIONALITY, READ_ACC, .init=setup_node, .fini=teardown_node, .timeout=5) {
    int movVal = 20;
    { // Node Instructions
        { // MOV 20 ACC
            Instruction inst = {
                .operation=MOV,
                
                .src.type=NUMBER,
                .src.value.dataVal=movVal,

                .dest.type=PORT,
                .dest.value.dataPort=ACC,
            };
            toTest->instructionList[0] = inst;
        }
        { // ADD ACC
            Instruction inst = {
                .operation=ADD,
                
                .src.type=PORT,
                .src.value.dataVal=ACC,
            };
            toTest->instructionList[1] = inst;
        }
        { // EXT
            Instruction inst = {
                .operation=EXT
            };
            toTest->instructionList[2] = inst;
        }
        toTest->instructionCount = 3;
    }
    thrd_t testThrd;

    threadArgs nodeArgs = {
        toTest,
    };

    cr_expect_neq(toTest->ACC, movVal);

    thrd_create(&testThrd, run_node, &nodeArgs);

    int res = 0;
    thrd_join(testThrd, &res);
    cr_expect_eq(res, SIGTERM);

    cr_expect_eq(toTest->ACC, movVal + movVal);
}

Test(NODE_FUNCTIONALITY, READ_NIL, .init=setup_node, .fini=teardown_node, .timeout=5) {
    int movVal = 20;
    { // Node Instructions
        { // MOV 20 ACC
            Instruction inst = {
                .operation=MOV,
                
                .src.type=NUMBER,
                .src.value.dataVal=movVal,

                .dest.type=PORT,
                .dest.value.dataPort=ACC,
            };
            toTest->instructionList[0] = inst;
        }
        { // ADD ACC
            Instruction inst = {
                .operation=ADD,
                
                .src.type=PORT,
                .src.value.dataVal=NIL,
            };
            toTest->instructionList[1] = inst;
        }
        { // EXT
            Instruction inst = {
                .operation=EXT
            };
            toTest->instructionList[2] = inst;
        }
        toTest->instructionCount = 3;
    }
    thrd_t testThrd;

    threadArgs nodeArgs = {
        toTest,
    };

    cr_expect_neq(toTest->ACC, movVal);

    thrd_create(&testThrd, run_node, &nodeArgs);

    int res = 0;
    thrd_join(testThrd, &res);
    cr_expect_eq(res, SIGTERM);

    cr_expect_eq(toTest->ACC, movVal);
}

Test(NODE_FUNCTIONALITY, READ_LAST, .timeout=5) {
    Node node1, node2;
    node_init(&node1);
    node_init(&node2);

    Pipe topPipe = {.data=NULL}, bottomPipe = {.data=NULL};
    sem_init(&topPipe.dataLock, 0, 1);
    sem_init(&bottomPipe.dataLock, 0, 1);

    node1.writePipes[RIGHT] = &topPipe;
    node2.readPipes[LEFT] = &topPipe;

    node2.writePipes[LEFT] = &bottomPipe;
    node1.readPipes[RIGHT] = &bottomPipe;
    
    { // Node 1
        {
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=5,

                .dest.type=PORT,
                .dest.value.dataPort=RIGHT
            };
            node1.instructionList[0] = inst;
        }
        {
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=10,

                .dest.type=PORT,
                .dest.value.dataPort=RIGHT
            };
            node1.instructionList[1] = inst;
        }
        {
            Instruction inst = {
                .operation=EXT,
            };
            node1.instructionList[2] = inst;
        }
        node1.instructionCount = 3;
    }
    { // Node 2
        {
            Instruction inst = {
                .operation=MOV,

                .src.type=NUMBER,
                .src.value.dataVal=5,

                .dest.type=PORT,
                .dest.value.dataPort=ACC
            };
            node2.instructionList[0] = inst;
        }
        {
            Instruction inst = {
                .operation=MOV,

                .src.type=PORT,
                .src.value.dataPort=LAST,

                .dest.type=PORT,
                .dest.value.dataPort=ACC
            };
            node2.instructionList[1] = inst;
        }
        {
            Instruction inst = {
                .operation=MOV,

                .src.type=PORT,
                .src.value.dataPort=ANY,

                .dest.type=PORT,
                .dest.value.dataPort=ACC
            };
            node2.instructionList[2] = inst;
        }
        {
            Instruction inst = {
                .operation=MOV,

                .src.type=PORT,
                .src.value.dataPort=LAST,

                .dest.type=PORT,
                .dest.value.dataPort=ACC
            };
            node2.instructionList[3] = inst;
        }
        {
            Instruction inst = {
                .operation=EXT,
            };
            node2.instructionList[4] = inst;
        }
        node2.instructionCount = 5;
    }

    // Start threads for nodes
    thrd_t node1Thrd, node2Thrd;

    threadArgs node1Args = {
        .node=&node1,
    };

    threadArgs node2Args = {
        .node=&node2,
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
    cr_expect_eq(node2.instructionPointer, 4);
    cr_expect_eq(node2.ACC, 10);
}
