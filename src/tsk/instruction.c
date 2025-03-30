#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

OPCode str_to_opcode(char* str) {
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
        if (0 == strcmp(OPCodeString[i], str)) {
            return i;
        }
    }

    return NOP;
}

Data str_to_data(char* str) {
    Data strData = {};
    if (NULL == str) {
        // Default to NIL for empty
        strData.type = PORT;
        strData.value.dataPort = NIL;
        return strData;
    }

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
        if (0 == strcmp(LocationStrings[i], str)) {
            strData.type = PORT;
            strData.value.dataPort = i;
            return strData;
        }
    }

    char *endPtr;
    long int potentialVal;

    // Check if this is a value or a label name.
    potentialVal = strtol(str, &endPtr, 10);
    if (endPtr == str) {
        // If this isn't valid with the opcode, check it later it's fine lol.
        strData.type = STRING;
        strData.value.dataStr = strdup(str);
        return strData;
    } else if (*endPtr != '\0') {
        printf("Invalid character: %c\n", *endPtr);
        exit(1);
    }

    strData.type = NUMBER;
    strData.value.dataVal = potentialVal;
    return strData;
}