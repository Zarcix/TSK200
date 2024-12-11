#include <stdlib.h>

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

static Node *node_get_neighbor(Node *currentNode, DirectionalLocation direction) {
    switch (direction) {
        case ANY: {
            return NULL;
        }
        case LAST: {
            return currentNode->lastNode;
        }
        case UP: case DOWN: case LEFT: case RIGHT: {
            return currentNode->pipes[direction];
        }
        case NIL: {
            return NULL;
        }
    }
}

// Parsers
static void parse_instruction_mov(Node *node, Data src, Data dest) {
    
}

static void parse_instruction_math(Node *node, OPCode operation, Data amount) {
    int totalAmount = 0;
    switch (operation) {
        case SUB: {

        }
        case ADD: {

        }
        case NOP: {
            totalAmount = 0;
            break;
        }
    }
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

    // Init ports
    for (int i = 0; i < PIPE_COUNT; i++) {
        node->pipes[i] = NULL;
    }
}

void node_parse_instruction(Node *node, Instruction input) {
    switch (input.operation) {
        case MOV: {
            // Parse Movement
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

void node_tick(Node *node) {
    node_set_instruction_pointer(node, node->instructionPointer + 1);
}

ReadResult node_read(Node *node, Data otherNode) {
    ReadResult result;
    result.isWaiting = false;

    if (node->isOutput) {
        return result;
    }

    switch (otherNode.type) {
        case VALUE: {
            result.value = otherNode.value.dataValue;
            break;
        }
        case LOCATION: {
            break;
        }
    }
}

void node_write(Node *node, DirectionalLocation dataDirection, int value) {

}

void node_cleanup(Node *node) {
    free(node->instructionList);
}