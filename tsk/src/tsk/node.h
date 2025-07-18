#ifndef NODE
#define NODE

#include <stdbool.h>

#include "./instruction.h"
#include "./pipe.h"
#include "./tsk_constants.h"

#include "../utils/linkedlist.h"
#include "../utils/hashmap.h"

/**
 * Represents a node
 */
typedef struct NodeStruct {
    // Instructions
    Instruction* instructionList;
    int instructionCount;
    int instructionPointer;
    struct hashmap_s labelMap;

    // Registers
    int ACC;
    int BAK;

    // Pipes
    Pipe* readPipes[PIPE_COUNT];
    Pipe* writePipes[PIPE_COUNT];

    // Psuedo Memory
    Port LAST;
} Node;


void node_init(Node *node);

void node_execute_instruction(Node *node, Instruction input);
void node_advance(Node *node);
void node_tick(Node *node);

int node_read(Node *node, Port readFrom);
void node_write(Node *node, Port writeTo, int value);

void node_cleanup(Node *node);

#endif
