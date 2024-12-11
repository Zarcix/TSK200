#ifndef NODE
#define NODE

#include <stdbool.h>

#include "operation_constants.h"

/**
 * Represents a node
 */
typedef struct {
    // Instructions
    Instruction *instructionList;
    unsigned int instructionCount;
    unsigned int instructionPointer;

    // Registers
    int ACC;
    int BAK;
    bool isOutput;

    // Pipes
    Node *pipes[4];

    // Psuedo Memory
    Node *lastNode;
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
void node_tick(Node *node);

ReadResult node_read(Node *node, Data otherNode);
void node_write(Node *node, DirectionalLocation dataDirection, int value);

void node_cleanup(Node *node);

#endif