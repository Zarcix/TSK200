#ifndef LINKEDLIST
#define LINKEDLIST

typedef struct _LinkedNode {
    int nodeValue;
    struct _LinkedNode *nextNode;
} LinkedNode;

typedef struct _LinkedList {
    LinkedNode *front;
    LinkedNode *back;
} LinkedList;

LinkedList* init_linked_list();
void destroy_linked_list(LinkedList *list);

void append_value(LinkedList *list, int value);
LinkedNode* pop_value(LinkedList *list);

#endif
