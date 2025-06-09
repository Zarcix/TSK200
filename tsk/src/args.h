#ifndef PROG_ARGS
#define PROG_ARGS

int parse_opt(int key, char *arg, struct argp_state *state);
int parse_args(int argc, char **argv);

#endif

