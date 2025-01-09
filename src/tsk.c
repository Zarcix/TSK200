#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "./tsk-src/node.h"
#include "./tsk-src/tsk_loader.h"

static bool PROGRAM_EXIT = false;
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

void* run_node(void* arg) {
    Node* node = (Node*)(arg);
    printf("Node index %p\n", (void*)node);
    return NULL;
}

int main(int argc, char **argv) {
    int *args = parse_args(argc, argv);

    int tickDelay = args[0];
    int maxOutputs = args[1];

    NodeList* node_mapping = tsk_to_node(COMMAND_PATH);
    
    int nodeCount = node_mapping->node_count;
    Node** node_list = node_mapping->node_list;

    pthread_t node_pids[nodeCount] = {};

    for (int i = 0; i < nodeCount; i++) {
        pthread_create(
            &node_pids[i], 
            NULL, 
            run_node, 
            node_list[i]
        );
        printf("%d: %p\n", i, (void*)node_list[i]);
    }

    for (int i = 0; i < nodeCount; i++) {
        pthread_join(node_pids[i], NULL);
    }
}
