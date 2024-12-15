#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#include "instruction.h"
#include "node.h"
#include "tsk_constants.h"

/* Instruction Helpers */

Instruction* parse_tsk_line(char* line) {
    Instruction* tskOperation = malloc(sizeof(Instruction));
    
    char* token = strtok(line, " ");

    if (token[0] && token[0] == '#') {
        return NULL;
    }

    for (int i = 0; i < 3; i++) {
        if (NULL == token) {
            break;
        }
        token[strcspn(token, "\n")] = 0;
        switch (i) {
            case 0: {
                tskOperation->operation = string_to_opcode(token);
                break;
            }
            case 1: {
                char *strEnd = NULL;
                int dataVal;
                dataVal = strtol(token, &strEnd, 10);

                if (strEnd == token) {
                    tskOperation->src.type = LOCATION;
                    tskOperation->src.value.nodeValue = string_to_direction(token);
                } else if (*strEnd != '\0') {
                    printf("Invalid Character %c\n", *strEnd);
                } else {
                    tskOperation->src.type = VALUE;
                    tskOperation->src.value.dataValue = dataVal;
                }
                break;
            }
            case 2: {
                char *strEnd = NULL;
                int dataVal;
                dataVal = strtol(token, &strEnd, 10);

                if (strEnd == token) {
                    tskOperation->dest.type = LOCATION;
                    tskOperation->dest.value.nodeValue = string_to_direction(token);
                } else if (*strEnd != '\0') {
                    printf("Invalid Character %c\n", *strEnd);
                } else {
                    tskOperation->dest.type = VALUE;
                    tskOperation->dest.value.dataValue = dataVal;
                }
                break;
            }
            default: {
                break;
            }
        }
        token = strtok(NULL, " ");
    }

    return tskOperation;
}

Node* parse_single_tsk_node(char* tskPath, char* tskNodeDef) {
    char newPath[MAX_COMMAND_LEN] = {};
    strcat(newPath, tskPath);
    strcat(newPath, "/");
    strcat(newPath, tskNodeDef);

    FILE *tskFile = fopen(newPath, "r");
    if (NULL == tskFile) {
        return NULL;
    }

    Node *newNode = malloc(sizeof(Node));
    node_init(newNode, false);

    char tskLine[MAX_COMMAND_LEN];
    Instruction *instructionList = calloc(MAX_INSTRUCTIONS, sizeof(Instruction));

    int instructionCounter = 0;
    while (NULL != fgets(tskLine, MAX_COMMAND_LEN, tskFile)) {
        Instruction* createdInstruction = parse_tsk_line(tskLine);
        if (NULL != createdInstruction) {
            instructionList[instructionCounter] = *createdInstruction;
            instructionCounter++;
        }
    }

    newNode->instructionList = instructionList;
    newNode->instructionCount = instructionCounter;

    fclose(tskFile);
    return newNode;
}

Node** read_tsk_nodes(char* tskPath) {
    printf("-- Finding Nodes --\n");
    DIR* tskD;
    struct dirent *tskDir;
    tskD = opendir(tskPath);

    int tskCount = 0;
    if (NULL == tskD) {
        printf("Path \"%s\" not found\n", tskPath);
        return NULL;
    }


    while (NULL != (tskDir = readdir(tskD))) {
        if (NULL != strstr(tskDir->d_name, ".tsk")) {
            printf("Node File Found: %s\n", tskDir->d_name);
            tskCount++;
        }
    }

    Node **nodeList = calloc(tskCount, sizeof(Node));
    rewinddir(tskD);
    int i = 0;

    while (NULL != (tskDir = readdir(tskD))) {
        if (NULL != strstr(tskDir->d_name, ".tsk")) {
            Node* newNode = parse_single_tsk_node(tskPath, tskDir->d_name);
            nodeList[i] = newNode;
            i++;
        }
    }

    // TODO Read Node Topology

    return nodeList;
}

/* Instruction Calls */

DirectionalLocation opposite_of_directional(DirectionalLocation originalDirection) {
    switch (originalDirection) {
        case LEFT: {
            return RIGHT;
        }
        case RIGHT: {
            return LEFT;
        }
        case UP: {
            return DOWN;
        }
        case DOWN: {
            return UP;
        }
        default: {
            return originalDirection;
        }
    }
}

Instruction** tsk_to_node(char* tskPath) {
    Instruction **instructionList = NULL;

    Node **nodes = read_tsk_nodes(tskPath);

    return instructionList;
}

OPCode string_to_opcode(char* inputString) {
    const char *OPCodeString[] = {
        "SUB",
        "ADD",
        "NEG",
        "NOP",

        "JEZ",
        "JMP",
        "JNZ",
        "JGZ",
        "JLZ",
        "JRO",

        "SAV",
        "SWP",

        "MOV"
    };

    int op_code_num = 0;
    while (NULL != OPCodeString[op_code_num]) {
        if (0 == strcmp(OPCodeString[op_code_num], inputString)) {
            return op_code_num;
        }
        op_code_num++;
    }

    return NOP;
}


DirectionalLocation string_to_direction(char* inputString) {
    const char *LocationStrings[] = {
        "LEFT",
        "RIGHT",
        "UP",
        "DOWN",
        "ANY",

        "NIL",
        "LAST",

        "ACC"
    };

    int op_code_num = 0;
    while (NULL != LocationStrings[op_code_num]) {
        if (0 == strcmp(LocationStrings[op_code_num], inputString)) {
            return op_code_num;
        }
        op_code_num++;
    }

    return NIL;
}
