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
    a->senderPipes[RIGHT] = c;
    a->senderPipes[UP] = d;
    a->senderPipes[DOWN] = e;

    node_write(a, LAST, 5);

    node_debug_print(a, "a");
    node_debug_print(b, "b");
}   