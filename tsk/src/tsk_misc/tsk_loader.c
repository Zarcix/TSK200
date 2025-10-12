#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "./tsk_loader.h"

#include "../tsk/node.h"
#include "../tsk/tsk_constants.h"
#include "../tsk/instruction.h"

#include "../utils/hashmap.h"
#include "../utils/strfun.h"

/* Base Parsers */

void parse_instruction_list(char* rawInstructionList[], Instruction* instructionListToWrite, int instructionCount) {
    for (int i = 0; i < instructionCount; i++) {
        // Init Stuff
        char opStr[MAX_STR_SIZE] = "";
        char srcStr[MAX_STR_SIZE] = "";
        char destStr[MAX_STR_SIZE] = "";

        instructionListToWrite[i].operation = (OPCode){0};
        instructionListToWrite[i].src = (Data){0};
        instructionListToWrite[i].dest = (Data){0};

        int opCount = sscanf(rawInstructionList[i], "%s %s %s", opStr, srcStr, destStr);

        if (opCount == EOF) {
            fprintf(stderr, "parse_instruction_list !! EOF issue reading the instruction line: %s\n", rawInstructionList[i]);
            exit(EXIT_FAILURE);
        }

        instructionListToWrite[i].operation = str_to_opcode(opStr);

        if (opCount <= 1) {
            continue;
        }

        instructionListToWrite[i].src = str_to_data(srcStr);

        if (opCount <= 2) {
            continue;
        }

        instructionListToWrite[i].dest = str_to_data(destStr);
    }
}

void parse_topology_map() {

}

/* Wrapper Functions for Specific Use */

// bool text_to_label(Node *parentNode, char *labelLine, int instructionCount) {
//     if (NULL != strstr(labelLine, ":")) {
//         return false;
//     }

//     char *labelName = strtok(labelLine, ":");
//     if (NULL != strstr(labelName, " ")) {
//         printf("read_instructions Error !! No spaces allowed in label name.");
//         exit(1);
//     }

//     char *mapKey = strdup(labelName);
//     int *mapVal = malloc(sizeof(int));
//     *mapVal = instructionCount;
//     if (0 != hashmap_put(&parentNode->labelMap, mapKey, strlen(labelName), mapVal)) {
//         printf("read_instructions Error !! Unable to add label '%s' to label hashmap", labelName);
//         exit(SIGABRT);
//     }

//     Data src = {
//         .type = STRING,
//         .value.dataStr = mapKey
//     };

//     Instruction labelInst = {
//         .operation = LABEL,
//         .src = src,
//     };

//     parentNode->instructionList[instructionCount] = labelInst;
//     return true;
// }

// void read_next_line(FILE *fd, char *section) {
//     while (fgets(section, MAX_STR_SIZE, fd)) {
//         // Remove trailing newline character, if any
//         section[strcspn(section, "\n")] = '\0';

//         // Skip empty lines
//         if (section[0] != '\0') {
//             return;
//         }
//     }

//     // If we reach EOF or no non-empty line is found, set section to empty
//     section[0] = '\0';
// }

// void read_instructions(Node* node, char *nodeName) {
//     // Read Node File
//     FILE *fd = fopen(nodeName, "r");

//     if (NULL == fd) {
//         printf("read_instructions Error !! Failed to read instructions from: %s\n", nodeName);
//         exit(SIGABRT);
//     }

//     int instructionCounter = 0;
//     while (!feof(fd)) {
//         char line[MAX_STR_SIZE] = "";
//         read_next_line(fd, line);
//         // If there is nothing after it's now null.
//         if (0 == strcmp(line, "")) {
//             break;
//         }

//         char *labelSep = line;
//         if (text_to_label(node, line, instructionCounter)) {
//             instructionCounter++;
//             continue;
//         }

//         if (NULL == labelSep) {
//             continue;
//         }

//         Instruction opInst = text_to_instruction(labelSep);

//         node->instructionList[instructionCounter] = opInst;
//         instructionCounter++;
//     }

//     node->instructionCount = instructionCounter;
//     fclose(fd);
// }

// void read_topology(const struct hashmap_s* const nodeMap, Node* node, char* nodeName) {
//     FILE *fd = fopen(nodeName, "r");

//     if (NULL == fd) {
//         printf("read_topology Error !! Failed to read topology from: %s\n", nodeName);
//         exit(SIGABRT);
//     }
    
//     char* token;
//     while (!feof(fd)) {
//         char line[MAX_STR_SIZE] = "";
//         read_next_line(fd, line);
//         // If there is nothing after it's now null.
//         if (0 == strcmp(line, "")) {
//             break;
//         }

//         // Grab tokens for parsing
//         token = strtok(line, ":");
//         char* directionToken = strdup(token);

//         token = strtok(NULL, ":");
//         char* otherNodeName = strdup(token);

//         token = strtok(NULL, ":");
//         if (NULL != token) {
//             printf("Extra tokens received: %s\n", token);
//         }

//         // Parse tokens
//         Port direction = str_to_port(directionToken);
//         Port otherDirection = reverse_port(direction);
//         Node* otherNode = hashmap_get(nodeMap, otherNodeName, strlen(otherNodeName));

//         if (NULL == otherNode) {
//             printf("Node '%s' not found. Exiting.\n", otherNodeName);
//             exit(SIGABRT);
//         }

//         // Check if a pipe has already been registered
//         if (NULL != node->writePipes[direction]) {
//             printf("Node '%s' already has a write pipe registered at %s. Exiting.\n", nodeName, PORT_AS_STR[direction]);
//             exit(SIGABRT);
//         }

//         if (NULL != otherNode->readPipes[otherDirection]) {
//             printf("Node '%s' already has a read pipe registered at %s. Exiting.\n", otherNodeName, PORT_AS_STR[otherDirection]);
//             exit(SIGABRT);
//         }

//         // Only write to write pipes. Freeing will also happen on write pipes only
//         Pipe* pipeConnector = malloc(sizeof(Pipe));

//         sem_init(&pipeConnector->dataLock, 0, 1);
//         pipeConnector->data = NULL;

//         node->writePipes[direction] = pipeConnector;
//         otherNode->readPipes[otherDirection] = pipeConnector;

//         free(directionToken);
//         free(otherNodeName);
//     }
//     fclose(fd);
// }


/* Public Functions */

void tsksrc_to_node(Node* node, const char* tskRawSrc) {
    // Form the raw instruction list
    int instructionCount = 0;
    for (int i = 0; tskRawSrc[i] != '\0'; i++) {
        if (tskRawSrc[i] == '\n') {
            instructionCount++;
        }
    }

    char *instructionList[instructionCount];

    char *srcCopy = strdup(tskRawSrc);
    char *line = strtok(srcCopy, "\n");
    int i = 0;
    while (line) {
        instructionList[i++] = strdup(line);
        line = strtok(NULL, "\n");
    }
    free(srcCopy);

    parse_instruction_list(instructionList, node->instructionList, instructionCount);

    node->instructionCount = instructionCount;

    for (int i = 0; i < instructionCount; i++) {
        printf("Node Instruction: %d\n", node->instructionList[i].operation);
    }
}

void tsktopo_link_node(const struct hashmap_s* const nodeMap, Node* node, char* nodeName) {
    // char topoPath[MAX_STR_SIZE] = "";
    // sprintf(topoPath, "%s.topo", nodeName);
    // read_topology(nodeMap, node, topoPath);
}
