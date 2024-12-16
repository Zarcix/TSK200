#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "node.h"
#include "instruction.h"
#include "tsk_constants.h"

/* Node Helper Functions */

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
        default: {
            break;
        }
    }
    return val;
}

/* Execution Helper Functions */

/** Execution for a MOV instruction
 *  
 * @param node: The node that has the move instruction to execute
 * @param src: The direction or value to move from
 * @param dest: The direction or value to move to
 */
static void execute_instruction_mov(Node *node, Data src, Data dest) {
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

/** Execution for math instrcutions
 * 
 * @param node: The node that has a math instruction to execute
 * @param operation: A math OPCode
 * @param amount: A direction or value that has the amount to perform
 */
static void execute_instruction_math(Node *node, OPCode operation, Data amount) {
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

static void execute_instruction_jump(Node *node, OPCode operation, Data label) {
    // int here to detect for negative jumps
    int newIP = 0;
    switch (operation) {
        case JEZ: {
            break;
        }
        case JMP: {
            break;
        }
        case JNZ: {
            break;
        }
        case JGZ: {
            break;
        }
        case JLZ: {
            break;
        }
        case JRO: {
            if (label.type == LABEL) {
                printf("Invalid type for JRO.");
                break;
            }

            int jumpOffset = node_get_data(node, label);
            if (node->isWaiting) {
                return;
            }

            newIP = node->instructionPointer + jumpOffset;
            break;
        }
        default: {
            break;
        }
    }

    // Check for underflows
    if (newIP - 1 < 0) {
        newIP = 0;
    }
    node->instructionPointer = newIP - 1;
}

/** Execution for register instructions
 * 
 * Register instructions are instructions that only affect the node's register
 * 
 * @param node: The node that has a register instruction to execute
 * @param operation: A register OPCode that the node will be executing
 */
static void execute_instruction_register(Node *node, OPCode operation) {
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

/** Initialize a node
 * 
 * @param node: The node to initialize
 * @param isOutputNode: Signifies if the node is an output node
 */
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

/** Parse a node instruction
 * 
 * @param node: The node containing the instruction
 * @param input: The instruction to parse
 */
void node_execute_instruction(Node *node, Instruction input) {
    switch (input.operation) {
        case MOV: {
            execute_instruction_mov(node, input.src, input.dest);
            break;
        }
        case SUB: case ADD: {
            execute_instruction_math(node, input.operation, input.src);
            break;
        }
        case JEZ: case JMP: case JNZ: case JGZ: case JLZ: case JRO: {
            execute_instruction_jump(node, input.operation, input.src);
            break;
        }
        case SAV: case SWP: case NEG: {
            execute_instruction_register(node, input.operation);
            break;
        }
        case NOP: default: {
            break;
        }
    }
}

/** Step an instruction in a node
 * 
 * @param node: The node to step an instruction
 */
void node_advance(Node *node) {
    if (!node->isWaiting) {
        node_set_instruction_pointer(node, node->instructionPointer + 1);
    }
}

void node_tick(Node *node) {
    // Ensure that the pointer loops back to the beginning
    if (node->instructionPointer >= node->instructionCount) {
        node->instructionPointer = 0;
    }
    Instruction toExecute = node->instructionList[node->instructionPointer];
    node_execute_instruction(node, toExecute);
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

/** Clean up node allocation
 * 
 * @param node: The node to clean
 */
void node_cleanup(Node *node) {
    free(node->instructionList);
}

/** Print the information contained in the node
 * 
 * @param node: The node to print info from
 * @param nodeName: An identifier for identification in console
 */
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
