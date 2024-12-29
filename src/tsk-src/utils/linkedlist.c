#include <stdlib.h>

#include "./linkedlist.h"

LinkedList* init_linked_list() {
    LinkedList *toReturn = malloc(sizeof(LinkedList));
    return toReturn;
}

void destroy_linked_list(LinkedList *list) {
    LinkedNode *toFree;

    while (NULL != list->front) {
        toFree = list->front;
        list->front = toFree->nextNode;
        free(toFree);
    }
}

void append_value(LinkedList *list, int value) {
    LinkedNode *newNode = malloc(sizeof(LinkedNode));
    newNode->nodeValue = value;

    LinkedNode *oldHead = list->front;
    newNode->nextNode = oldHead;
    
    list->front = newNode;
}

LinkedNode* pop_value(LinkedList *list) {
    LinkedNode *frontNode = list->front;

    if (NULL == frontNode) {
        return NULL;
    }

    list->front = frontNode->nextNode;

    return frontNode;
}
