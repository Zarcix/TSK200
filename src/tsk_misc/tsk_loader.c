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

void read_next_line(FILE *fd, char *section) {
    while (fgets(section, MAX_STR_SIZE, fd)) {
        // Remove trailing newline character, if any
        section[strcspn(section, "\n")] = '\0';

        // Skip empty lines
        if (section[0] != '\0') {
            return;
        }
    }

    // If we reach EOF or no non-empty line is found, set section to empty
    section[0] = '\0';
}

void read_instructions(Node* node, char *nodeName) {
    // Read Node File
    FILE *fd = fopen(nodeName, "r");

    if (NULL == fd) {
        printf("read_instructions Error !! Failed to read instructions from: %s\n", nodeName);
        exit(SIGABRT);
    }

    int instructionCounter = 0;
    while (!feof(fd)) {
        char line[MAX_STR_SIZE] = "";
        read_next_line(fd, line);
        // If there is nothing after it's now null.
        if (0 == strcmp(line, "")) {
            break;
        }

        char *labelSep = line;
        if (NULL != strstr(line, ":")) {
            labelSep = strtok(line, ":");
            if (NULL != strstr(labelSep, " ")) {
                printf("read_instructions Error !! No spaces allowed in label name.");
                exit(SIGABRT);
            }
            
            char *mapKey = strdup(labelSep);
            int *mapVal = malloc(sizeof(int));
            *mapVal = instructionCounter;
            if (0 != hashmap_put(&node->labelMap, mapKey, strlen(labelSep), mapVal)) {
                printf("read_instructions Error !! Unable to add label '%s' to label hashmap", labelSep);
                exit(SIGABRT);
            }

            Data src = {
                .type = STRING,
                .value.dataStr = strdup(labelSep)
            };

            Instruction labelInst = {
                .operation = LABEL,
                .src = src,
            };

            node->instructionList[instructionCounter] = labelInst;

            labelSep = strtok(NULL, "");
            instructionCounter++;
        }

        if (NULL == labelSep) {
            continue;
        }

        trim_str(labelSep);

        char *op_str = strtok(labelSep, " ");
        char *src_str = strtok(NULL, " ");
        char *dest_str = strtok(NULL, " ");

        OPCode opcode = str_to_opcode(op_str);
        Data src = str_to_data(src_str);
        Data dest = str_to_data(dest_str);

        Instruction opInst = {
            .operation = opcode,
            .src = src,
            .dest = dest,
        };

        node->instructionList[instructionCounter] = opInst;
        instructionCounter++;
    }

    node->instructionCount = instructionCounter;
    fclose(fd);
}

void read_topology(const struct hashmap_s* const nodeMap, Node* node, char* nodeName) {
    FILE *fd = fopen(nodeName, "r");

    if (NULL == fd) {
        printf("read_topology Error !! Failed to read topology from: %s\n", nodeName);
        exit(SIGABRT);
    }
    
    char* token;
    while (!feof(fd)) {
        char line[MAX_STR_SIZE] = "";
        read_next_line(fd, line);
        // If there is nothing after it's now null.
        if (0 == strcmp(line, "")) {
            break;
        }

        // Grab tokens for parsing
        token = strtok(line, ":");
        char* directionToken = strdup(token);

        token = strtok(NULL, ":");
        char* otherNodeName = strdup(token);

        token = strtok(NULL, ":");
        if (NULL != token) {
            printf("Extra tokens received: %s\n", token);
        }

        // Parse tokens
        Port direction = str_to_port(directionToken);
        Port otherDirection = reverse_port(direction);
        Node* otherNode = hashmap_get(nodeMap, otherNodeName, strlen(otherNodeName));

        if (NULL == otherNode) {
            printf("Node '%s' not found. Exiting.\n", otherNodeName);
            exit(SIGABRT);
        }

        // Check if a pipe has already been registered
        if (NULL != node->writePipes[direction]) {
            printf("Node '%s' already has a write pipe registered at %s. Exiting.\n", nodeName, PORT_AS_STR[direction]);
            exit(SIGABRT);
        }

        if (NULL != otherNode->readPipes[otherDirection]) {
            printf("Node '%s' already has a read pipe registered at %s. Exiting.\n", otherNodeName, PORT_AS_STR[otherDirection]);
            exit(SIGABRT);
        }

        // Only write to write pipes. Freeing will also happen on write pipes only
        Pipe* pipeConnector = malloc(sizeof(Pipe));

        sem_init(&pipeConnector->dataLock, 0, 1);
        pipeConnector->data = NULL;

        node->writePipes[direction] = pipeConnector;
        otherNode->readPipes[otherDirection] = pipeConnector;

        free(directionToken);
        free(otherNodeName);
    }
    fclose(fd);
}

/* Public Functions */

void tsksrc_to_node(Node* node, char* nodeName) {
    char topoPath[MAX_STR_SIZE] = "";
    sprintf(topoPath, "%s.tsk", nodeName);
    read_instructions(node, topoPath);
}

void tsktopo_link_node(const struct hashmap_s* const nodeMap, Node* node, char* nodeName) {
    char topoPath[MAX_STR_SIZE] = "";
    sprintf(topoPath, "%s.topo", nodeName);
    read_topology(nodeMap, node, topoPath);
}
