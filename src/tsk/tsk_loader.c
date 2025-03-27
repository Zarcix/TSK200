#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./node.h"
#include "./tsk_constants.h"
#include "./instruction.h"
#include "./tsk_loader.h"

void read_next_line(FILE *fd, char *section) {
    fgets(section, MAX_SECTION_NAME_LEN, fd);

    // Skip over empty lines
    while (0 == strcmp(section, "\n")) {
        fgets(section, MAX_SECTION_NAME_LEN, fd);
    }
}

void read_instructions(FILE *fd) {
    Instruction *instructionList = malloc(sizeof(Instruction) * MAX_INSTRUCTIONS);
    int instructionListCount = 0;

    int found_instruction = 1;
    while (!feof(fd) && found_instruction > 0) {
        // Read Line
        char line[MAX_COMMAND_LEN];
        read_next_line(fd, line);
        
        // Parse Line
        char opcode[MAX_STR_SIZE], src[MAX_STR_SIZE], dest[MAX_STR_SIZE];
        char label[MAX_STR_SIZE];
        found_instruction = sscanf(line, "%s %s %s", opcode, src, dest);
        if (found_instruction <= 0) {
            found_instruction = sscanf(line, "%[^:]:", label);
        }

        // Exit on a section ender
        if (0 == strcmp(opcode, "@!")) break;
        
        if (instructionListCount > MAX_INSTRUCTIONS) {
            printf("read_instructions !! Too many instructions to parse.");
            exit(1);
        }

        printf("Opcode: %s\n", opcode);
        printf("SRC: %s\n", src);
        printf("DEST: %s\n", dest);
        printf("\n");
    }
}

void read_topology(FILE *fd) {

}

/* Public Functions */

Node* tsksrc_to_node(char* filename) {
    // Open file for parsing
    FILE *fd = fopen(filename, "r");

    if (fd == NULL) {
        printf("tsksrc_to_node !! Failed to read %s\n", filename);
        exit(1);
    }

    while (!feof(fd)) {
        // Read Line
        char line[MAX_COMMAND_LEN];
        read_next_line(fd, line);
        
        // Read Section Name
        char sectionName[MAX_SECTION_NAME_LEN];
        int found = sscanf(line, "!@%s", sectionName);
        if (found <= 0) {
            printf("tsksrc_to_node !! Configuration section not found.");
            exit(1);
        }

        // Parse Section
        printf("%s\n", sectionName);
        if (0 == strcmp(sectionName, "INST")) {
            read_instructions(fd);
        } else if (0 == strcmp(sectionName, "TOPO")) {
            read_topology(fd);
        }
    }

    // Node* tskNode = malloc(sizeof(Node));
    
    return NULL;
}