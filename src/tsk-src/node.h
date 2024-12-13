#ifndef NODE
#define NODE

#include <stdbool.h>

#include "instruction.h"

/**
 * Represents a node
 */
typedef struct NodeStruct {
    // Instructions
    Instruction *instructionList;
    unsigned int instructionCount;
    unsigned int instructionPointer;

    // Registers
    int ACC;
    int BAK;
    bool isOutput;

    // Pipes
    struct NodeStruct *currentPipe[4];
    struct NodeStruct *senderPipes[4];
    int senderData[4];

    // Psuedo Memory
    DirectionalLocation LAST;
} Node;

/**
 * Represents a result from a read.
 * 
 * @param isWaiting: Set to true if the value is not ready to be read
 * @param value: Value read
 */
typedef struct {
    bool isWaiting;
    int value;
} ReadResult;

void node_init(Node *node, bool isOutputNode);

void node_parse_instruction(Node *node, Instruction input);
void node_advance(Node *node);
void node_tick(Node *node);

ReadResult node_read(Node *node, DirectionalLocation dataDirection);
void node_write(Node *node, DirectionalLocation dataDirection, int value);

void node_cleanup(Node *node);

// Debugs
void node_debug_print(Node *node, char* nodeName);

#endif
