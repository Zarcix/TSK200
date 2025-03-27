#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <argp.h>

#include "./tsk/node.h"
#include "./tsk/tsk_loader.h"

const int PARSABLE_ARGS = 3;

static char* COMMAND_PATH = "";

static int NODE_MAX_OUTPUTS = 0;
static int TICK_DELAY = 0;

int main(int argc, char **argv) {
    tsksrc_to_node("node.tsk");
}
