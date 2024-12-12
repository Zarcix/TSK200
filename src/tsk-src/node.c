#include <stdlib.h>
#include <stdio.h>

#include "node.h"
#include "tsk_constants.h"

/* Helper Functions */

// Node Helpers
static void node_set_instruction_pointer(Node *node, int newPointer) {
    if (newPointer >= node->instructionCount || newPointer < 0) {
        newPointer = 0;
    }

    node->instructionPointer = newPointer;
}

// Parsers
static void parse_instruction_mov(Node *node, Data src, Data dest) {
    // int sourceVal = 0;
    // switch (src.type) {
    //     case VALUE: {
    //         sourceVal = src.value.dataValue;
    //         break;
    //     }
    //     case LOCATION: {
    //         NodeData *srcData = node_get_neighbor_data(node, src.value.nodeValue);
    //         if (srcData && srcData->hasData) {
    //             sourceVal = srcData->data;
    //         }
    //         break;
    //     }
    // }


    // printf("%d\n", sourceVal);
}

static void parse_instruction_math(Node *node, OPCode operation, Data amount) {
    // int totalAmount = 0;
    // switch (operation) {
    //     case SUB: {
    //         if (amount.type == VALUE) {
    //             totalAmount = amount.value.dataValue;
    //         } else {
                
    //         }
    //         break;
    //     }
    //     case ADD: {
            
    //         break;
    //     }
    //     case NOP: default: {
    //         break;
    //     }
    // }
    // node->ACC += totalAmount;
}

static void parse_instruction_jump(Node *node, OPCode operation, Data label) {

}

static void parse_instruction_register(Node *node, OPCode operation) {
    // switch (operation) {
    //     case SAV: {
    //         node->BAK = node->ACC;
    //         break;
    //     }
    //     case SWP: {
    //         int tmp = node->BAK;
    //         node->BAK = node->ACC;
    //         node->ACC = tmp;
    //         break;
    //     }
    //     case NEG: {
    //         node->ACC *= -1;
    //         break;
    //     }
    //     default: {
    //         break;
    //     }
    // }
}

/* Node Calls */
void node_init(Node *node, bool isOutputNode) {
    // Init instructions
    node->instructionCount = 0;
    node->instructionPointer = 0;
    node->instructionList = calloc(MAX_INSTRUCTIONS, sizeof(Instruction));

    // Init registers
    node->ACC = 0;
    node->BAK = 0;
    node->isOutput = isOutputNode;

    // Init ports
    for (int i = 0; i < PIPE_COUNT; i++) {
        node->senderPipes[i] = NULL;
    }
}

void node_parse_instruction(Node *node, Instruction input) {
    // switch (input.operation) {
    //     case MOV: {
    //         parse_instruction_mov(node, input.src, input.dest);
    //         break;
    //     }
    //     case SUB: case ADD: case NOP: {
    //         // Parse Math
    //         break;
    //     }
    //     case JEZ: case JMP: case JNZ: case JGZ: case JLZ: case JRO: {
    //         // Parse Jump
    //         break;
    //     }
    //     case SAV: case SWP: case NEG: {
    //         parse_instruction_register(node, input.operation);
    //         break;
    //     }
    // }
}

void node_advance(Node *node) {
    node_set_instruction_pointer(node, node->instructionPointer + 1);
}

void node_tick(Node *node) {
    node_advance(node);
}

ReadResult node_read(Node *node, DirectionalLocation dataDirection, Data otherData) {
    ReadResult result;
    result.isWaiting = false;
    return result;
}

/**
 * Write data into a node's pipe
 * 
 * @param node: The node that's writing the data
 * @param dataDirection: The direction to write the data to
 * @param value: The data to write
 */
void node_write(Node *node, DirectionalLocation dataDirection, int value) {
    switch (dataDirection) {
        case LEFT: case RIGHT: case UP: case DOWN: {
            Node *toWrite = node->senderPipes[dataDirection];
            if (toWrite == NULL) {
                break;
            }
            node->currentPipe[dataDirection] = toWrite;
            node->senderData[dataDirection] = value;
            break;
        }
        case ANY: {
            DirectionalLocation anyOrder[4] = {
                LEFT, RIGHT, UP, DOWN
            };
            for (int i = 0; i < 4; i++) {
                node_write(node, anyOrder[i], value);
            }
            break;
        }
        case ACC: {
            node->ACC = value;
            break;
        }
        case LAST: {
            if (node->lastNode == NULL) {
                break;
            }
            node->currentPipe[dataDirection] = node->lastNode;
            node->senderData[dataDirection] = value;
            break;
        }
        case NIL: default: {
            break;
        }
    }
}

void node_cleanup(Node *node) {
    free(node->instructionList);
}

void node_debug_print(Node *node, char* nodeName) {
    printf("Node: %s\n", nodeName);

    // Internals
    printf("\tACC: %d\n", node->ACC);
    printf("\tBAK: %d\n", node->BAK);

    printf("\n");

    // Ports
    printf("\tLeft Pipe: %d\n", node->senderData[LEFT]);
    printf("\tRight Pipe: %d\n", node->senderData[RIGHT]);
    printf("\tUp Pipe: %d\n", node->senderData[UP]);
    printf("\tDown Pipe: %d\n", node->senderData[DOWN]);
}