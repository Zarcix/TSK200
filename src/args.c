#include <argp.h>
#include <string.h>
#include <libgen.h>

#include "./constants.h"

#include "./tsk/node.h"

static struct argp_option program_options[] = {
    { "verbose", 'v', 0, 0, "Node(s) to use with TSK"},
    { 0 }
};

static int parse_opt(int key, char* arg, struct argp_state *state) {
    switch (key) {
        case 'v': {
            NODE_OUTPUT = true;
            break;
        }
        case ARGP_KEY_ARG: {
            Node *currentNode = malloc(sizeof(Node));
            node_init(currentNode);
            
            int nameLength = strcspn(arg, ".");
            char nodeName[nameLength];
            strncpy(nodeName, arg, nameLength);

            hashmap_put(&NODE_MAPS, strdup(nodeName), strlen(nodeName), currentNode);
            break;
        }
    }
    return 0;
}

static int parse_args(int argc, char **argv) {
    init_constants();
    struct argp argp = { program_options, parse_opt, "NODE"};
    return argp_parse (&argp, argc, argv, 0, 0, 0);
}
