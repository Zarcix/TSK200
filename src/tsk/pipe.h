/**
    A connection between two nodes.
*/
typedef struct {
    volatile int data;
    volatile bool hasData;
} Pipe;