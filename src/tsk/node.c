#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./node.h"
#include "./instruction.h"
#include "./tsk_constants.h"

void parse_math_instruction(Node *node, Instruction inst) {
    switch (inst.operation) {
        case ADD: {
            Data src = inst.src;
            if (src.type == STRING) {
                printf("parse_math_instruction ADD !! Invalid SRC, is STRING\n");
                exit(1);
            }
            
            int data = 0;

            if (src.type == PORT) {
                data = node_read(node, src.value.dataPort);
            } else if (src.type == NUMBER) {
                data = src.value.dataVal;
            }

            node->ACC += data;
            break;
        }
        case SUB: {
            Data src = inst.src;
            if (src.type == STRING) {
                printf("parse_math_instruction SUB !! Invalid SRC, is STRING\n");
                exit(1);
            }
            
            int data = 0;

            if (src.type == PORT) {
                data = node_read(node, src.value.dataPort);
            } else if (src.type == NUMBER) {
                data = src.value.dataVal;
            }

            node->ACC -= data;
            break;
        }
        case NEG: {
            node->ACC *= -1;
            break;
        }
        case NOP: default: {
            break;
        }
    }
}

void parse_node_instruction(Node *node, OPCode opcode) {
    switch (opcode) {
        case SWP: {
            int tmp = node->ACC;
            node->ACC = node->BAK;
            node->BAK = tmp;
            break;
        }
        case SAV: {
            node->BAK = node->ACC;
            break;
        }
        default: {
            printf("parse_node_instruction {-} !! Invalid Instruction: %d\n", opcode);
            exit(1);
        }
    }
}

void parse_instruction_manipulation(Node *node, Instruction inst) {
    switch(inst.operation) {
        case JMP: {
            break;
        }
        case JEZ: {
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
            Data src = inst.src;
            if (src.type == STRING) {
                printf("parse_instruction_manipulation JRO !! Invalid SRC, is STRING\n");
                exit(1);
            }
            
            int data = 0;

            if (src.type == PORT) {
                data = node_read(node, src.value.dataPort);
            } else if (src.type == NUMBER) {
                data = src.value.dataVal;
            }

            node->instructionPointer += data;
            break;
        }
        default: {
            printf("parse_instruction_manipulation {-} !! Invalid Instruction: %d\n", inst.operation);
            exit(1);
        }
    }
}

void parse_io_instruction(Node *node, Instruction inst) {
    // Read Data First
    Data src = inst.src;
    int value = 0;
    switch (src.type) {
        case STRING: {
            printf("parse_io_instruction STRING !! Invalid SRC Value, found STRING\n");
            exit(1);
        }
        case NUMBER: {
            value = src.value.dataVal;
            break;
        }
        case PORT: {
            value = node_read(node, src.value.dataPort);
            break;
        }
    }

    // Write Data After
    Data dest = inst.dest;
    if (dest.type != PORT) {
        printf("parse_io_instruction {-} !! Invalid REGISTER\n");
        exit(1);
    }
    node_write(node, dest.value.dataPort, value);
}

/** Initialize a node
 * 
 * @param node: The node to initialize
 * @param isOutputNode: Signifies if the node is an output node
 */
void node_init(Node *node) {
    // Init instructions
    node->instructionList = NULL;
    node->instructionCount = 0;
    node->instructionPointer = 0;

    // Init registers
    node->ACC = 0;
    node->BAK = 0;
    node->LAST = NIL;

    // Init ports
    for (int i = 0; i < PIPE_COUNT; i++) {
        node->connectedPipes[i] = NULL;
    }
}

/** Parse a node instruction
 * 
 * @param node: The node containing the instruction
 * @param input: The instruction to parse
 */
void node_execute_instruction(Node *node, Instruction input) {
    switch (input.operation) {
        case NOP: case ADD: case SUB: case NEG: {
            parse_math_instruction(node, input);
            break;
        }
        case JMP: case JEZ: case JNZ: case JGZ: case JLZ: case JRO: {
            parse_instruction_manipulation(node, input);
            break;
        }
        case MOV: {
            parse_io_instruction(node, input);
            break;
        }
        case SWP: case SAV: {
            parse_node_instruction(node, input.operation);
            break;
        }
        case LABEL: default: {
        }
    }
}

/** Step an instruction in a node
 * 
 * @param node: The node to step an instruction
 */
void node_advance(Node *node) {
    node->instructionPointer++;
}

void node_tick(Node *node) {
    node_advance(node);

    if (node->instructionPointer < 0) {
        node->instructionPointer = 0;
    }
}

/** Reads data from a node's pipe
 * @param node: The node that's reading data
 * @param readFrom: The direction to read data from
 */
int node_read(Node *node, Port readFrom) {
    int value = 0;
    switch (readFrom) {
        case ACC: {
            return node->ACC;
        }
        case NIL: {
            return 0;
        }
        case LAST: {
            int val = node_read(node, node->LAST);
            return val;
        }
        default: {
        }
    }

    return value;
}

/** Write data into a node's pipe
 * @param node: The node that's writing the data
 * @param writeTo: The direction to write the data to
 * @param value: The data to write
 */
void node_write(Node *node, Port writeTo, int value) {
    // Quit early when writing to internal ports
    switch (writeTo) {
        case ACC: {
            node->ACC = value;
            return;
        }
        case NIL: {
            return;
        }
        case LAST: {
            node_write(node, node->LAST, value);
            return;
        }
        default: {
        }
    }

    // TODO handle directionals here
}

/** Clean up node allocation
 * 
 * @param node: The node to clean
 */
void node_cleanup(Node *node) {
    free(node->instructionList);
}