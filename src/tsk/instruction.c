#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

OPCode string_to_opcode(char* str) {
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
    Data strData;

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
    potentialVal = strtol(str, &endPtr, 10);
    if (endPtr == str) {
        printf("No digits found in input\n");
        exit(1);
    } else if (*endPtr != '\0') {
        printf("Invalid character: %c\n", *endPtr);
        exit(1);
    }

    strData.type = NUMBER;
    strData.value.dataVal = potentialVal;
    return strData;
}