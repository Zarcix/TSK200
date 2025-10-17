#include <stdio.h>
#include <stdlib.h>

#include "../../utils/clog.h"
#include "../../tsk/tsk_constants.h"

#include "parse_instruction.h"

Instruction parse_single_instruction(char instruction[]) {
    Instruction newInstruction = (Instruction){0};

    char opStr[MAX_STR_SIZE] = "";
    char srcStr[MAX_STR_SIZE] = "";
    char destStr[MAX_STR_SIZE] = "";

    int opCount = sscanf(instruction, "%s %s %s", opStr, srcStr, destStr);

    if (opCount == EOF) {
        FATAL("Reading instruction hit EOF: %s", instruction);
        exit(EXIT_FAILURE);
    }

    newInstruction.operation = str_to_opcode(opStr);

    if (opCount > 1) {
        newInstruction.src = str_to_data(srcStr);
    }

    if (opCount > 2) {
        newInstruction.dest = str_to_data(destStr);
    }

    return newInstruction;
}

void parse_instruction_list(int instructionCount, char* instructionList[instructionCount], Instruction instructionWriteList[]) {
    for (int i = 0; i < instructionCount; i++) {
        Instruction newInstruction = parse_single_instruction(instructionList[i]);
        instructionWriteList[i] = newInstruction;
    }
}