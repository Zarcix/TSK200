#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./node.h"
#include "./tsk_constants.h"

Instruction* parse_tsk_line(char* line) {
    if (NULL == line) {
        return NULL;
    }

    Instruction* tskOperation = malloc(sizeof(Instruction));

    if (strstr(line, ":")) {
        char* token = strtok(line, ":");
        token[strcspn(token, "\n")] = 0;
        tskOperation->operation = OPLBL;
        tskOperation->src.type = LABEL;
        tskOperation->src.value.label = token;
        printf("'%s'\n", token);
        return tskOperation;
    }

    char* token = strtok(line, " ");
    token[strcspn(token, "\n")] = 0;

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
    Instruction *instructionList = malloc(MAX_INSTRUCTIONS * sizeof(Instruction));

    int instructionCounter = 0;
    while (NULL != fgets(tskLine, MAX_COMMAND_LEN, tskFile)) {
        Instruction* createdInstruction = parse_tsk_line(tskLine);
        if (NULL != createdInstruction && instructionCounter < MAX_INSTRUCTIONS) {
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

    return nodeList;
}

Node** tsk_to_node(char* tskPath) {
    Node **nodes = read_tsk_nodes(tskPath);

    return nodes;
}
