#include <stdlib.h>
#include <stdio.h>

#include "./tsk-src/node.h"

int main(int argc, char **argv) {
    Node *a = malloc(sizeof(Node));
    Node *b = malloc(sizeof(Node));
    Node *c = malloc(sizeof(Node));
    Node *d = malloc(sizeof(Node));
    Node *e = malloc(sizeof(Node));

    a->senderPipes[LEFT] = b;
    b->senderPipes[RIGHT] = a;

    a->senderPipes[RIGHT] = c;
    c->senderPipes[LEFT] = a;

    a->senderPipes[UP] = d;
    d->senderPipes[DOWN] = a;

    a->senderPipes[DOWN] = e;
    e->senderPipes[UP] = a;

    ReadResult res;

    // Step 1
    node_write(d, DOWN, 1);

    // Step 2
    res = node_read(a, ANY);
    node_write(a, RIGHT, res.value);

    // Step 3
    node_write(d, DOWN, 1);
    res = node_read(c, LEFT);
    node_write(c, ACC, res.value);

    // Step 4
    res = node_read(a, LAST);
    node_write(a, DOWN, res.value);

    // Step 5
    res = node_read(e, UP);
    node_write(e, ACC, res.value);

    // Step 6
    Instruction negInstruction;
    negInstruction.operation = NEG;
    node_parse_instruction(c, negInstruction);

    Instruction swpInstruction;
    swpInstruction.operation = SWP;
    node_parse_instruction(c, swpInstruction);

    node_debug_print(e, "E");
    node_debug_print(c, "C");
}   