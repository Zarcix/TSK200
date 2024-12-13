#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "node.h"
#include "instruction.h"
#include "tsk_constants.h"

/* Helper Functions */

// Node Helpers
static void node_set_instruction_pointer(Node *node, int newPointer) {
    if (newPointer >= node->instructionCount || newPointer < 0) {
        newPointer = 0;
    }

    node->instructionPointer = newPointer;
}

static int node_get_data(Node *node, Data dataValue) {
    int val = 0;
    switch (dataValue.type) {
        case VALUE: {
            val = dataValue.value.dataValue;
            break;
        }
        case LOCATION: {
            ReadResult srcData = node_read(node, dataValue.value.nodeValue);
            if (srcData.isWaiting) {
                node->isWaiting = true;
                break;
            }
            node->isWaiting = false;
            val = srcData.value;
            break;
        }
    }
    return val;
}

// Parsers
static void parse_instruction_mov(Node *node, Data src, Data dest) {
    // Preprocessing Checks
    if (dest.type != LOCATION) {
        node->hasError = true;
        node->errorMessage = "MOV Destination Invalid: Not a location";
        return;
    }

    // MOV Parsing
    int srcVal = node_get_data(node, src);
    if (node->isWaiting) {
        return;
    }

    node_write(node, dest.value.nodeValue, srcVal);
}

static void parse_instruction_math(Node *node, OPCode operation, Data amount) {
    int totalAmount = 0;

    // Used for ADD and SUB
    int mathMultipler = 1;
    if (operation == SUB) {
        mathMultipler = -1;
    }

    switch (operation) {
        case ADD: case SUB: {
            if (amount.type == VALUE) {
                totalAmount = amount.value.dataValue * mathMultipler;
                break;
            }
            int dataVal = node_get_data(node, amount);
            if (node->isWaiting) {
                return;
            }
            totalAmount = dataVal * mathMultipler;
            break;
        }
        default: {
            break;
        }
    }
    node->ACC += totalAmount;
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
    node->isWaiting = false;

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
        case SUB: case ADD: {
            parse_instruction_math(node, input.operation, input.src);
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
        case NOP: default: {
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

    printf("\n");

    // Errors
    printf("\tError: %d\n", node->hasError);
    printf("\tError Message: %s\n", node->errorMessage);

    printf("\n");

    // Others
    printf("\tWaiting: %d\n", node->isWaiting);
}
