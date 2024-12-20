#include <string.h>
#include <dirent.h>
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
    int JumpInstructionCount = 6;
    const OPCode JumpInstructionList[] = {
        JEZ,
        JMP,
        JNZ,
        JGZ,
        JLZ,
        JRO
    };

    for (int i = 0; i < JumpInstructionCount; i++) {
        if (operation == JumpInstructionList[i]) {
            return true;
        }
    }

    return false;
}

OPCode string_to_opcode(char* inputString) {
    const int OPCodeCount = 14;
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

    for (int i = 0; i < OPCodeCount; i++) {
        if (0 == strcmp(OPCodeString[i], inputString)) {
            return i;
        }
    }

    return NOP;
}


DirectionalLocation string_to_direction(char* inputString) {
    const int LocationCount = 8;
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

    for (int i = 0; i < LocationCount; i++) {
        if (0 == strcmp(LocationStrings[i], inputString)) {
            return i;
        }
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
