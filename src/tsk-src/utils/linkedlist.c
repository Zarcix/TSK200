#include <stdlib.h>

#include "./linkedlist.h"

LinkedList* init_linked_list(int value) {
    LinkedList *toReturn = malloc(sizeof(LinkedList));
    
    LinkedNode *newNode = malloc(sizeof(LinkedNode));
    newNode->nodeValue = value;
    newNode->nextNode = NULL;

    toReturn->front = newNode;
    toReturn->back = newNode;

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

int pop_value(LinkedList *list) {
    LinkedNode *frontNode = list->front;

    list->front = frontNode->nextNode;

    int nodeValue = frontNode->nodeValue;
    free(frontNode);

    return nodeValue;
}