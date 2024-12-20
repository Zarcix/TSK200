#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "./tsk-src/node.h"
#include "./tsk-src/tsk_loader.h"

static char* COMMAND_PATH = "";
const int PARSABLE_ARGS = 3;

int* parse_args(int argc, char **argv) {
    // argList[0] = delay time
    // arglist[1] = max output
    int* argList = malloc(PARSABLE_ARGS * sizeof(int));
    argList[0] = -1;
    argList[1] = -1;

    bool seenArgParam = false;
    int argType = 0;

    for (int argIndex = 1; argIndex < argc; ++argIndex) {
        char* argument = argv[argIndex];

        if (seenArgParam && argType == 2) {
            // Parse Strings
            COMMAND_PATH = argument;
            seenArgParam = false;
            continue;
        }

        if (seenArgParam) {
            // Parse Ints
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
        } else if (0 == strcmp(argument, "--path")) {
            seenArgParam = true;
            argType = 2;
        }
    }

    return argList;
}

int main(int argc, char **argv) {
    int *args = parse_args(argc, argv);
    int tickDelay = args[0];
    int maxOutputs = args[1];

    Node** nodeList = tsk_to_node(COMMAND_PATH);
    int nodeIndex = 0;
    if (NULL == nodeList[nodeIndex]) {
        return -1;
    }
    while (true) {
        if (NULL == nodeList[nodeIndex]) {
            nodeIndex = 0;
        }
        Node *currentNode = nodeList[nodeIndex];

        if (currentNode->isOutput && maxOutputs > 0 && currentNode->outputCount >= maxOutputs) {
            break;
        }

        if (tickDelay > 0) {
            sleep(tickDelay);
        }

        node_tick(currentNode);
        nodeIndex++;
    }
}
