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
    fgets(section, MAX_STR_SIZE, fd);
    if (0 == strcmp(section, "")) {
        section = "";
        return;
    }

    // Skip over empty lines
    while (0 == strcmp(section, "\n")) {
        fgets(section, MAX_STR_SIZE, fd);
    }
    section[strcspn(section, "\n")] = '\0';
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
}

void read_topology(FILE *fd) {
    // TODO Once I figure out how tf to get MOV working
}

/* Public Functions */

void tsksrc_to_node(Node* node, char* nodeName) {
    char topoPath[MAX_STR_SIZE];
    sprintf(topoPath, "%s.tsk", nodeName);
    read_instructions(node, topoPath);
}

