#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./node.h"
#include "./tsk_constants.h"
#include "./instruction.h"
#include "./tsk_loader.h"

/* Helper Functions */

Instruction* parse_tsk_line(char* line) {
    if (NULL == line) {
        return NULL;
    }

    Instruction* tskOperation = malloc(sizeof(Instruction));
    char* smth = strstr(line, ":");
    if (smth) {
        char* token = strtok(line, ":");
        token[strcspn(token, "\n")] = 0;
        tskOperation->operation = OPLBL;
        tskOperation->src.type = LABEL;
        tskOperation->src.value.label = malloc(MAX_LABEL_LEN);
        strcpy(tskOperation->src.value.label, token);
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

                if (opcode_is_jump_instruction(tskOperation->operation)) {
                    tskOperation->src.type = LABEL;
                    tskOperation->src.value.label = malloc(MAX_LABEL_LEN);
                    strcpy(tskOperation->src.value.label, token);
                    break;
                }

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

Node* get_node_from_node_list(char* nodeToFind, int nodeListCount) {
    Node* foundNode = NULL;
    for (int i = 0; i < nodeListCount; i++) {
        if (strcmp(node_mappings[i].name, nodeToFind) == 0) {
            foundNode = node_mappings[i].node;
            break;
        }
    }
    return foundNode;
}

void parse_tsk_topology(char* tskPath, char* currentNode, int nodeListCount) {
    char newPath[MAX_COMMAND_LEN] = {};
    strcat(newPath, tskPath);
    strcat(newPath, "/");
    strcat(newPath, currentNode);

    FILE *tskFile = fopen(newPath, "r");
    if (NULL == tskFile) {
        return;
    }

    char tskLine[MAX_COMMAND_LEN];
    while (NULL != fgets(tskLine, MAX_COMMAND_LEN, tskFile)) {
        // Remove comments and strip newlines
        char *ptr = strchr(tskLine, '#');
        if (NULL != ptr) {
            *ptr = '\0';
        }
        tskLine[strcspn(tskLine, "\n")] = 0;

        char* locationToken = strtok(tskLine, ":");
        if (NULL == locationToken) {
            continue;
        }
        char* nodeToken = strtok(NULL, ":");
        if (NULL == nodeToken) {
            continue;
        }

        DirectionalLocation location = string_to_direction(locationToken);
        if (!direction_is_node_direction(location) && strcmp(locationToken, "OUTPUT") != 0) {
            printf("Location not a location nor output, skipping rest of file\n");
            return;
        }

        ptr = strchr(currentNode, '.');
        if (NULL != ptr) {
            *ptr = '\0';
        }

        ptr = strchr(nodeToken, '.');
        if (NULL != ptr) {
            *ptr = '\0';
        }

        Node *myNode = get_node_from_node_list(currentNode, nodeListCount);
        if (NULL == myNode) {
            printf("Current node is not valid\n");
            return;
        }

        if (strcmp(locationToken, "OUTPUT") == 0) {
            myNode->isOutput = true;
            continue;
        }

        Node *otherNode = get_node_from_node_list(nodeToken, nodeListCount);
        if (NULL == otherNode) {
            printf("Other node is not valid\n");
            return;
        }

        
        myNode->senderPipes[location] = otherNode;
    }
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
        // Remove comments
        char *ptr = strchr(tskLine, '#');
        if (NULL != ptr) {
            *ptr = '\0';
        }
        tskLine[strcspn(tskLine, "\n")] = 0;

        if (0 == strcmp(tskLine, "")) {
            continue;
        }
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
    DIR* tskD;
    struct dirent *tskDir;
    tskD = opendir(tskPath);

    int tskCount = 0;
    if (NULL == tskD) {
        printf("Directory \"%s\" not found\n", tskPath);
        return NULL;
    }
    
    // Count the amount of tsk nodes
    while (NULL != (tskDir = readdir(tskD))) {
        if (NULL != strstr(tskDir->d_name, ".tsk")) {
            tskCount++;
        }
    }

    Node **nodeList = calloc(tskCount, sizeof(Node));

    node_mappings = malloc(tskCount * sizeof(NodeMapIndex));
    node_mapping_count = tskCount;
    rewinddir(tskD);
    int i = 0;

    // Create nodes based on tsk instructions
    while (NULL != (tskDir = readdir(tskD))) {
        if (NULL != strstr(tskDir->d_name, ".tsk")) {
            Node* newNode = parse_single_tsk_node(tskPath, tskDir->d_name);
            nodeList[i] = newNode;

            char *ptr = strchr(tskDir->d_name, '.');
            if (NULL != ptr) {
                *ptr = '\0';
            }

            node_mappings[i].name = malloc(256 * sizeof(char));
            strcpy(node_mappings[i].name, tskDir->d_name);
            node_mappings[i].node = newNode;

            i++;
        }
    }

    rewinddir(tskD);

    // Connect nodes together based on topology
    while (NULL != (tskDir = readdir(tskD))) {
        if (NULL != strstr(tskDir->d_name, ".topo")) {
            parse_tsk_topology(tskPath, tskDir->d_name, tskCount);
        }
    }

    return nodeList;
}

/* Public Functions */

Node** tsk_to_node(char* tskPath) {
    Node **nodes = read_tsk_nodes(tskPath);

    return nodes;
}

char* tsk_get_node_name(Node* nodeToSearch) {
    char *nodeName = "\0";
    for (int i = 0; i < node_mapping_count; i++) {
        if (node_mappings[i].node == nodeToSearch) {
            nodeName = node_mappings[i].name;
        }
    }
    return nodeName;
}
