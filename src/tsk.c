#include <stdlib.h>

#include "./tsk-src/node.h"
#include "tsk-src/instruction.h"

void test_node_read_write() {
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

void test_node_mov_val_acc() {
    Node *center = malloc(sizeof(Node));
    Node *left = malloc(sizeof(Node));

    center->senderPipes[LEFT] = left;
    left->senderPipes[RIGHT] = center;

    Instruction movInstructionCenter;
        movInstructionCenter.operation = MOV;
        {
            movInstructionCenter.src.type = LOCATION;
            movInstructionCenter.src.value.dataValue = LEFT;
        }
        {
            movInstructionCenter.dest.type = LOCATION;
            movInstructionCenter.dest.value.nodeValue = ACC;
        }

    Instruction movInstructionLeft;
        movInstructionLeft.operation = MOV;
        {
            movInstructionLeft.src.type = VALUE;
            movInstructionLeft.src.value.dataValue = 5;
        }
        {
            movInstructionLeft.dest.type = LOCATION;
            movInstructionLeft.dest.value.nodeValue = RIGHT;
        }

    node_parse_instruction(center, movInstructionCenter);
    node_parse_instruction(left, movInstructionLeft);
    node_parse_instruction(center, movInstructionCenter);

    node_debug_print(center, "Center");
}

void test_node_add_val() {
    Node *center = malloc(sizeof(Node));
    Node *left = malloc(sizeof(Node));

    center->senderPipes[LEFT] = left;
    left->senderPipes[RIGHT] = center;

    Instruction centerList[2];
    { // Center Instructions
        Instruction addInstruction;
            addInstruction.operation = ADD;
            {
                addInstruction.src.type = VALUE;
                addInstruction.src.value.dataValue = 5;
            }
        Instruction movInstructionCenter;
            movInstructionCenter.operation = ADD;
            {
                movInstructionCenter.src.type = LOCATION;
                movInstructionCenter.src.value.dataValue = NIL;
            }
        centerList[0] = addInstruction;
        centerList[1] = movInstructionCenter;
    }

    Instruction leftList[1];
    { // Left Instructions
        Instruction movInstructionLeft;
            movInstructionLeft.operation = MOV;
            {
                movInstructionLeft.src.type = VALUE;
                movInstructionLeft.src.value.dataValue = 2;
            }
            {
                movInstructionLeft.dest.type = LOCATION;
                movInstructionLeft.dest.value.nodeValue = RIGHT;
            }
        leftList[0] = movInstructionLeft;
    }

    center->instructionList = centerList;
    left->instructionList = leftList;

    node_parse_instruction(center, centerList[0]);
    node_parse_instruction(left, leftList[0]);
    node_parse_instruction(center, centerList[1]);

    node_debug_print(center, "Center");
}

int main(int argc, char **argv) {
    test_node_add_val();
}
