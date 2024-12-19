#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "./tsk-src/node.h"
#include "./tsk-src/tsk_loader.h"

const int MAX_ARGS = 2;
int* parse_args(char **argv) {
    // argList[0] = delay time
    // arglist[1] = max output
    int* argList = malloc(MAX_ARGS * sizeof(int));
    argList[0] = 0;
    argList[1] = -1;

    bool seenArgParam = false;
    int argIndex = 1;
    int argType = 0;

    while (NULL != argv[argIndex]) {
        char* argument = argv[argIndex];
        argIndex++;

        if (seenArgParam) {
            char *endCheck;
            int num = strtol(argument, &endCheck, 10);

            if (*endCheck != '\0') {
                printf("Invalid input.\n");
                continue;
            }
            argList[argType] = num;
            seenArgParam = false;
        }

        if (0 == strcmp(argument, "--delay-time")) {
            seenArgParam = true;
            argType = 0;
        } else if (0 == strcmp(argument, "--max-outputs")) {
            seenArgParam = true;
            argType = 1;
        }
    }

    return argList;
}

int main(int argc, char **argv) {
    int *args = parse_args(argv);
    int tickDelay = args[0];
    int maxOutputs = args[1];

    Node** nodeList = tsk_to_node("sigh-commands");
    int nodeIndex = 0;
    while (true) {
        if (NULL == nodeList[nodeIndex]) {
            nodeIndex = 0;
        }
        Node *currentNode = nodeList[nodeIndex];

        if (currentNode->isOutput && maxOutputs > 0 && currentNode->outputCount >= maxOutputs) {
            break;
        }

        sleep(tickDelay);
        node_tick(currentNode);
        nodeIndex++;
    }
}
