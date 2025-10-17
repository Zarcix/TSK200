#include <argp.h>
#include <string.h>
#include <libgen.h>
#include <libfyaml.h>

#include "./args.h"
#include "./constants.h"

#include "./tsk/node.h"

#include "./tsk_misc/tsk_loader.h"

static struct argp_option program_options[] = {
    { "verbose", 'v', 0, 0, "Node(s) to use with TSK" },
    { "help", 'h', 0, OPTION_HIDDEN, "Provide help list" },
    { "config", 'c', "FILE", 0, "Config file containing a TSK layout" },
    { 0 }
};

void parse_yaml_doc(struct fy_document *fyd) {
    struct fy_node *yaml_root = fy_document_root(fyd);

    struct fy_node *nodeList = fy_node_mapping_lookup_by_string(yaml_root, "nodeList", -1);
    struct fy_node *nodeInstructions = fy_node_mapping_lookup_by_string(yaml_root, "nodeInstructions", -1);
    struct fy_node *nodeMapping = fy_node_mapping_lookup_by_string(yaml_root, "nodeMapping", -1);

    if (!nodeList) {
        fprintf(stderr, "No `nodeList` found in config file.\n");
    }

    if (!nodeInstructions) {
        fprintf(stderr, "No `nodeInstructions` found in config file.\n");
    }

    if (!nodeMapping) {
        fprintf(stderr, "No `nodeMapping` found in config file.\n");
    }

    /* Start Parsing Node List */
    void *iter = NULL;
    struct fy_node *current_entry;
    while ((current_entry = fy_node_sequence_iterate(nodeList, &iter)) != NULL) {
        /* Create and Initialize the Node first */
        size_t nodeNameLength = 0;
        const char *nodeNameRaw = fy_node_get_scalar(current_entry, &nodeNameLength);
        if (!nodeNameRaw) {
            continue;
        }

        char nodeName[nodeNameLength + 1];
        strncpy(nodeName, nodeNameRaw, nodeNameLength);
        nodeName[nodeNameLength] = '\0';

        Node *newNode = malloc(sizeof(Node));
        node_init(newNode);

        int ret = hashmap_put(&NODE_MAPS, strdup(nodeName), nodeNameLength + 1, newNode);
        if (ret) {
            fprintf(stderr, "Failed putting node into hashmap\n");
        }

        /* Add Node Instructions to created Nodes */
        struct fy_node *instructionList = fy_node_mapping_lookup_by_string(nodeInstructions, nodeName, nodeNameLength + 1);
        if (!instructionList) {
            printf("No instructions listed for node %s\n", nodeName);
            continue;
        }

        size_t instructionListLen = 0;
        const char *nodeInstructions = fy_node_get_scalar(instructionList, &instructionListLen);
        if (nodeInstructions) {
            tsksrc_to_node(newNode, nodeInstructions);
        }
        /* Map the nodes together*/
    }


}

int parse_opt(int key, char* arg, struct argp_state *state) {
    switch (key) {
        case 'v': {
            printf("Verbose Mode Activated.\n");
            NODE_OUTPUT = true;
            break;
        }
        case 'h': {
            argp_state_help(state, stdout, ARGP_HELP_STD_HELP); 
            break;
        }
        case 'c': {
            struct fy_document *fyd = NULL;
            fyd = fy_document_build_from_file(NULL, arg);
            if (!fyd) {
                fprintf(stderr, "Failed to build yaml document from: %s\n", arg);
            }
            parse_yaml_doc(fyd);
            fy_document_destroy(fyd);
            break;
        }
        // case ARGP_KEY_ARG: {
        //     Node *currentNode = malloc(sizeof(Node));
        //     node_init(currentNode);
            
        //     int nameLength = strcspn(arg, ".");
        //     char nodeName[nameLength + 1];
        //     strncpy(nodeName, arg, nameLength);
        //     nodeName[nameLength] = '\0';

        //     hashmap_put(&NODE_MAPS, strdup(nodeName), nameLength, currentNode);
        //     break;
        // }
    }
    return 0;
}

int parse_args(int argc, char **argv) {
    struct argp argp = { program_options, parse_opt, 0};
    return argp_parse(&argp, argc, argv, 0, 0, 0);
}
