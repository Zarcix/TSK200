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
    switch (operation) {
        case SAV: {
            node->BAK = node->ACC;
            break;
        }
        case SWP: {
            int tmp = node->BAK;
            node->BAK = node->ACC;
            node->ACC = tmp;
            break;
        }
        case NEG: {
            node->ACC *= -1;
            break;
        }
        default: {
            break;
        }
    }
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
    node->LAST = NIL;

    // Init ports
    for (int i = 0; i < PIPE_COUNT; i++) {
        node->senderPipes[i] = NULL;
    }
}

void node_parse_instruction(Node *node, Instruction input) {
    switch (input.operation) {
        case MOV: {
            parse_instruction_mov(node, input.src, input.dest);
            break;
        }
        case SUB: case ADD: case NOP: {
            // Parse Math
            break;
        }
        case JEZ: case JMP: case JNZ: case JGZ: case JLZ: case JRO: {
            // Parse Jump
            break;
        }
        case SAV: case SWP: case NEG: {
            parse_instruction_register(node, input.operation);
            break;
        }
    }
}

void node_advance(Node *node) {
    node_set_instruction_pointer(node, node->instructionPointer + 1);
}

void node_tick(Node *node) {
    node_advance(node);
}

/** Reads data from a node's pipe
 * @param node: The node that's reading data
 * @param dataDirection: The direction to read data from
 */
ReadResult node_read(Node *node, DirectionalLocation dataDirection) {
    ReadResult result;
    result.isWaiting = false;
    DirectionalLocation oppositeDirection = opposite_of_directional(dataDirection);

    switch (dataDirection) {
        case LEFT: case RIGHT: case UP: case DOWN: {
            Node *toRead = node->senderPipes[dataDirection];
            if (toRead == NULL) {
                result.isWaiting = true;
                break;
            }

            if (toRead->currentPipe[oppositeDirection] == NULL) {
                result.isWaiting = true;
                break;
            }

            // Get Data
            result.value = toRead->senderData[oppositeDirection];

            // Reset sender for all pipes
            for (int i = 0; i < AnyOrderCount; i++) {
                toRead->currentPipe[i] = NULL;
                toRead->senderData[i] = 0;
            }

            break;
        }
        case ANY: {
            // Read nodes in order.
            for (int i = 0; i < AnyOrderCount; i++) {
                result = node_read(node, AnyOrder[i]);
                if (!result.isWaiting) {
                    node->LAST = AnyOrder[i];
                    return result;
                }
            }
            result.isWaiting = true;
            break;
        }
        case ACC: {
            result.value = node->ACC;
            break;
        }
        case LAST: {
            result = node_read(node, node->LAST);
            break;
        }
        case NIL: {
            result.value = 0;
            break;
        }
        default: {
            break;
        }
    }

    return result;
}

/** Write data into a node's pipe
 * @param node: The node that's writing the data
 * @param dataDirection: The direction to write the data to
 * @param value: The data to write
 */
void node_write(Node *node, DirectionalLocation dataDirection, int value) {
    for (int i = 0; i < AnyOrderCount; i++) {
        if (node->currentPipe[i] != NULL) {
            return;
        }
    }
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
            for (int i = 0; i < AnyOrderCount; i++) {
                node_write(node, AnyOrder[i], value);
            }
            break;
        }
        case ACC: {
            node->ACC = value;
            break;
        }
        case LAST: {
            node_write(node, node->LAST, value);
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
    printf("\tLeft Pipe Val: %d\n", node->senderData[LEFT]);
    printf("\tLeft Pipe Set: %d\n", node->currentPipe[LEFT] != NULL);

    printf("\tRight Pipe Val: %d\n", node->senderData[RIGHT]);
    printf("\tRight Pipe Set: %d\n", node->currentPipe[RIGHT] != NULL);

    printf("\tUp Pipe Val: %d\n", node->senderData[UP]);
    printf("\tUp Pipe Set: %d\n", node->currentPipe[UP] != NULL);

    printf("\tDown Pipe Val: %d\n", node->senderData[DOWN]);
    printf("\tDown Pipe Set: %d\n", node->currentPipe[DOWN] != NULL);
}
