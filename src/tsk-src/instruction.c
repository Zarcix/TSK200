#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>

#include "instruction.h"

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

bool opcode_is_jump_instruction(OPCode operation) {
    int instructionCount = 6;
    const OPCode JumpInstructionList[] = {
        JEZ,
        JMP,
        JNZ,
        JGZ,
        JLZ,
        JRO
    };

    for (int i = 0; i < instructionCount; i++) {
        if (operation == JumpInstructionList[i]) {
            return true;
        }
    }

    return false;
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

        "MOV",
        "OPLBL"
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

bool direction_is_node_direction(DirectionalLocation directionInQuestion) {
    switch (directionInQuestion) {
        case LEFT:
        case RIGHT:
        case UP:
        case DOWN: {
            return true;
        }
        default: {
            return false;
        }
    }
}
