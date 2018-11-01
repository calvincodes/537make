//
// Created by Arpit Jain on 11/1/18.
//

#include <stdlib.h>
#include <stdio.h>
#include "LinkedList.h"

int main() {
    LLNode* head = createNode("arpit");
    append(head, "jain");
    printf("end");
}

LLNode *createNode(char *element) {

    LLNode* llNode = (LLNode *) malloc(sizeof(LLNode));

    if(!llNode){
        fprintf(stderr, "Could not allocate memory for LinkedList data");
        exit(1);
    }

    llNode->element = element;
    llNode->next = NULL;
    return llNode;
}

void append(LLNode *node, char *element) {

    if (node == NULL) {
        fprintf(stderr, "Null node passed to append function");
    }

    LLNode* newNode = createNode(element);
    node->next = newNode;
    newNode->next = NULL;
}

LLNode *getElement(LLNode *node, int index) {
    // TODO: Implement if required
}