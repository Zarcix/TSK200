#include <semaphore.h>

/**
    A connection between two nodes.
*/
typedef struct {
    int* data;
    sem_t dataLock;
} Pipe;