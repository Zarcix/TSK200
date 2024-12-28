#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h> 

#include "./tsk-src/node.h"
#include "./tsk-src/tsk_loader.h"

static Node** NodeList; 
static pthread_mutex_t NodeLock;

static bool PROGRAM_EXIT = false;
static int *ARGS;
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

void* run_tsk(void *param) {
    int tickDelay = ARGS[0];
    int maxOutputs = ARGS[1];

    int tskIndex = *(int*)param;
    Node *currentNode = NodeList[tskIndex];

    if (NULL == currentNode) {
        return NULL;
    }

    while (true) {
        if (PROGRAM_EXIT) {
            break;
        }

        if (currentNode->isOutput && maxOutputs > 0 && currentNode->outputCount >= maxOutputs - 1) {
            PROGRAM_EXIT = true;
        }

        if (tickDelay > 0) {
            sleep(tickDelay);
        }

        node_tick(currentNode);
    }

    return NULL;
}

int main(int argc, char **argv) {
    ARGS = parse_args(argc, argv);

    Node** nodeList = tsk_to_node(COMMAND_PATH);
    int nodeIndex = 0;
    if (NULL == nodeList[nodeIndex]) {
        return -1;
    }

    NodeList = nodeList;
    if (pthread_mutex_init(&NodeLock, NULL) != 0) {
        printf("Mutex initialization failed. Exiting\n");
        return -1;
    }

    int nodeCount = 0;
    while (NULL != NodeList[nodeCount]) {
        nodeCount++;
    }

    pthread_t threadList[nodeCount];

    for (int i = 0; i < nodeCount; i++) {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&(threadList[i]), NULL, &run_tsk, arg);
    }

    for (int i = 0; i < nodeCount; i++) {
        pthread_join(threadList[i], NULL);
    }

    pthread_mutex_destroy(&NodeLock);
}
