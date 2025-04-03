#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "./node.h"
#include "./instruction.h"
#include "./tsk_constants.h"

void node_jump_to_label(Node *node, Instruction instruction) {
    if (instruction.src.type != STRING) {
        printf("Invalid label. Not a string.\n");
        exit(SIGABRT);
    }

    char* labelName = instruction.src.value.dataStr;

    void* idx_void = hashmap_get(&node->labelMap, labelName, strlen(labelName));
    if (NULL == idx_void) {
        printf("Invalid label name: %s\n", labelName);
        exit(SIGABRT);
    }

    unsigned int label_idx = *(unsigned int*)idx_void;
    node->instructionPointer = label_idx;
}

/** INSTRUCTION SPECIFIC PARSERS **/

/** Parse a given math instruction
        - Format: <OPCODE> <SRC>
        - Supported OPCodes: 
            1. ADD
            2. SUB
            3. NEG
            4. NOP
        - Supported SRC Types:
            1. PORT
            2. NUMBER
        - Supported DEST Types:
            DEST is not supported 
*/
void parse_math_instruction(Node *node, Instruction inst) {
    switch (inst.operation) {
        case ADD: {
            Data src = inst.src;
            if (src.type == STRING) {
                printf("parse_math_instruction ADD !! Invalid SRC, is STRING\n");
                exit(SIGABRT);
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
                exit(SIGABRT);
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

/** Parse a given node instruction
        - Format: <OPCODE>
        - Supported OPCodes: 
            1. SAV
            2. SWP
        - Supported SRC Types:
            SRC is not supported 
        - Supported DEST Types:
            DEST is not supported 
*/
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
            exit(SIGABRT);
        }
    }
}

/** Parse an instruction that changes the node's instruction pointer
        - Format: <OPCODE> <SRC>
        - Supported OPCodes: 
            1. JMP
            2. JEZ
            3. JNZ
            4. JGZ
            5. JLZ
            6. JRO
        - Supported SRC Types:
            1. STRING
            2. NUMBER (JRO)
            3. PORT (JRO)
        - Supported DEST Types:
            DEST is not supported 
*/
void parse_inst_pointer_instruction(Node *node, Instruction inst) {
    switch(inst.operation) {
        case JMP: {
            node_jump_to_label(node, inst);
            break;
        }
        case JEZ: {
            if (node->ACC == 0) {
                node_jump_to_label(node, inst);
            }
            break;
        }
        case JNZ: {
            if (node->ACC != 0) {
                node_jump_to_label(node, inst);
            }
            break;
        }
        case JGZ: {
            if (node->ACC > 0) {
                node_jump_to_label(node, inst);
            }
            break;
        }
        case JLZ: {
            if (node->ACC < 0) {
                node_jump_to_label(node, inst);
            }
            break;
        }
        case JRO: {
            Data src = inst.src;
            if (src.type == STRING) {
                printf("parse_instruction_manipulation JRO !! Invalid SRC, is STRING\n");
                exit(SIGABRT);
            }
            
            int data = 0;
            if (src.type == PORT) {
                data = node_read(node, src.value.dataPort);
            } else if (src.type == NUMBER) {
                data = src.value.dataVal;
            }

            // Pad since node has to advance after
            node->instructionPointer += data;

            if (node->instructionPointer < 0) {
                node->instructionPointer = 0;
                break;
            }

            if (node->instructionPointer >= node->instructionCount) {
                node->instructionPointer = node->instructionCount - 1;
                break;
            }

            // Skip over labels
            while (node->instructionList[node->instructionPointer].operation == LABEL) {
                if (node->instructionPointer < 0) {
                    node->instructionPointer = 0;
                    break;
                }

                if (node->instructionPointer >= node->instructionCount) {
                    node->instructionPointer = node->instructionCount - 1;
                    break;
                }

                int pointerMovement = data < 0 ? -1 : 1;
                node->instructionPointer += pointerMovement;
            }

            break;
        }
        default: {
            printf("parse_instruction_manipulation {-} !! Invalid Instruction: %d\n", inst.operation);
            exit(SIGABRT);
        }
    }
}

/** Parse an instruction that deal with value movement
        - Format: <OPCODE> <SRC> <DEST>
        - Supported OPCodes:
            1. MOV
        - Supported SRC Types:
            1. NUMBER
            2. PORT
        - Supported DEST Types:
            2. PORT

    Note:
        Using a PORT in SRC will cause the node to become blocking on reading.

*/
void parse_io_instruction(Node *node, Instruction inst) {
    // Read Data First
    Data src = inst.src;
    int value = 0;

    if (inst.operation != MOV) {
        printf("Invalid IO Instruction.\n");
        exit(SIGABRT);
    }

    switch (src.type) {
        case STRING: {
            printf("parse_io_instruction STRING !! Invalid SRC Value, found STRING\n");
            exit(SIGABRT);
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
        exit(SIGABRT);
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
    node->instructionList = malloc(sizeof(Instruction) * MAX_INSTRUCTIONS);
    node->instructionCount = 0;
    node->instructionPointer = 0;
    hashmap_create(INIT_MAP_SIZE, &node->labelMap);

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
            parse_inst_pointer_instruction(node, input);
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
    bool toAdvance = true;

    // JRO handles it's own jumping individually
    if (node->instructionList[node->instructionPointer].operation == JRO) {
        toAdvance = false;
    }

    node_execute_instruction(node, node->instructionList[node->instructionPointer]);

    if (toAdvance) {
        node_advance(node);
    }


    if (node->instructionPointer < 0 || node->instructionPointer >= node->instructionCount) {
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
            if (node->LAST == LAST) {
                printf("Loop detected on READ. LAST port is LAST");
                exit(SIGABRT);
            }
            int val = node_read(node, node->LAST);
            return val;
        }
        case ANY: {
            bool dataFound = false;
            Pipe *pipePtr = NULL;

            while (!dataFound) {
                for (int i = 0; i < EXTERNAL_PORT_COUNT; i++) {
                    pipePtr = node->connectedPipes[EXTERNAL_PORT_LIST[i]];

                    if (NULL == pipePtr) {
                        continue;
                    }

                    sem_wait(&pipePtr->dataLock);
                    if (NULL == pipePtr->data) {
                        sem_post(&pipePtr->dataLock);
                        continue;
                    }
                    dataFound = true;
                    value = *pipePtr->data;
                    free(pipePtr->data);
                    sem_post(&pipePtr->dataLock);
                }
            }
            break;
        }
        case LEFT: case RIGHT: case UP: case DOWN: {
            Pipe* pipePtr = node->connectedPipes[readFrom];

            // Loop forever when data is not available
            while (NULL == pipePtr);

            sem_wait(&pipePtr->dataLock);
            while (NULL == pipePtr->data) {
                sem_post(&pipePtr->dataLock);
                sem_wait(&pipePtr->dataLock);
            }

            value = *pipePtr->data;
            free(pipePtr->data);
            sem_post(&pipePtr->dataLock);
            break;
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
            if (node->LAST == LAST) {
                printf("Loop detected on WRITE. LAST port is LAST");
                exit(SIGABRT);
            }
            node_write(node, node->LAST, value);
            return;
        }
        case ANY: {
            break;
        }
        case LEFT: case RIGHT: case UP: case DOWN: {
            break;
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
    hashmap_destroy(&node->labelMap);

    for (int i = 0; i < PIPE_COUNT; i++) {
        free(node->connectedPipes[i]);
    }
}