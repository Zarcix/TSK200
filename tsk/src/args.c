#include <argp.h>
#include <string.h>
#include <libgen.h>

#include "./args.h"
#include "./constants.h"

#include "./tsk/node.h"

static struct argp_option program_options[] = {
    { "verbose", 'v', 0, 0, "Node(s) to use with TSK"},
    { 0 }
};

int parse_opt(int key, char* arg, struct argp_state *state) {
    switch (key) {
        case 'v': {
            printf("Verbose Mode Activated.\n");
            NODE_OUTPUT = true;
            break;
        }
        case ARGP_KEY_ARG: {
            Node *currentNode = malloc(sizeof(Node));
            node_init(currentNode);
            
            int nameLength = strcspn(arg, ".");
            char nodeName[nameLength + 1];
            strncpy(nodeName, arg, nameLength);
            nodeName[nameLength] = '\0';

            hashmap_put(&NODE_MAPS, strdup(nodeName), nameLength, currentNode);
            break;
        }
    }
    return 0;
}

int parse_args(int argc, char **argv) {
    struct argp argp = { program_options, parse_opt, "NODE"};
    return argp_parse (&argp, argc, argv, 0, 0, 0);
}
