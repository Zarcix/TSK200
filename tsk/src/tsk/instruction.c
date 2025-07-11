#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "./instruction.h"

const char* OPCODE_AS_STR[] = {
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
	"LABEL",
	"EXT"
};

const char* PORT_AS_STR[] = {
	"LEFT",
	"RIGHT",
	"UP",
	"DOWN",
	"ANY",

	"NIL",
	"LAST",

	"ACC"
};

OPCode str_to_opcode(char* str) {
    for (int i = 0; i < OPCODE_COUNT; i++) {
        if (0 == strcmp(OPCODE_AS_STR[i], str)) {
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

    for (int i = 0; i < PORT_COUNT; i++) {
        if (0 == strcmp(PORT_AS_STR[i], str)) {
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
        exit(SIGABRT);
    }

    strData.type = NUMBER;
    strData.value.dataVal = potentialVal;
    return strData;
}

Port str_to_port(char* str) {
    for (int i = 0; i < PORT_COUNT; i++) {
        if (0 == strcmp(PORT_AS_STR[i], str)) {
            return i;
        }
    }

    return ANY;
}

Port reverse_port(Port port) {
    switch (port) {
        case UP: {
            return DOWN;
        }
        case DOWN: {
            return UP;
        }
        case LEFT: {
            return RIGHT;
        }
        case RIGHT: {
            return LEFT;
        }
        default: {
            return port;
        }
    }
}
