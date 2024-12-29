#ifndef NODE
#define NODE

#include <stdbool.h>

#include "instruction.h"
#include "./utils/linkedlist.h"

typedef enum {
    RUN,
    WRITE,
    READ
} NodeState;

typedef enum {
    EXEC,
    STACK,
    OUTPUT,
} NodeType;

typedef union {
    long int outputCount;
    LinkedList *stack;
} NodeData;

/**
 * Represents a node
 */
typedef struct NodeStruct {
    // Instructions
    Instruction *instructionList;
    unsigned int instructionCount;
    unsigned int instructionPointer;
    NodeState state;

    // Registers
    int ACC;
    int BAK;

    // Node Types
    NodeType type;
    NodeData typeData;

    // Pipes
    bool currentPipe[4];
    struct NodeStruct *senderPipes[4];
    int senderData[4];

    // Psuedo Memory
    DirectionalLocation LAST;
    
    // Error Handling
    bool hasError;
    char *errorMessage;
} Node;


void node_init(Node *node, NodeType nodeType);

void node_execute_instruction(Node *node, Instruction input);
void node_advance(Node *node);
void node_tick(Node *node);

int node_read(Node *node, DirectionalLocation dataDirection);
void node_write(Node *node, DirectionalLocation dataDirection, int value);

void node_cleanup(Node *node);

// Debugs
void node_debug_print(Node *node, char* nodeName);

#endif
