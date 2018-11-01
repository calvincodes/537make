//
// Created by Arpit Jain on 11/1/18.
//

#include <stdlib.h>
#include <stdio.h>
#include "LinkedList.h"

int main_1() {
    LLNode* head = createLLNode("arpit");
    appendToLL(head, "jain");
    printf("end");
}

LLNode *createLLNode(char *element) {

    LLNode* llNode = (LLNode *) malloc(sizeof(LLNode));

    if(!llNode){
        fprintf(stderr, "Could not allocate memory for LinkedList data");
        exit(1);
    }

    llNode->element = element;
    llNode->next = NULL;
    return llNode;
}

void appendToLL(LLNode *node, char *element) {

    if (node == NULL) {
        fprintf(stderr, "Null node passed to appendToLL function");
    }

    LLNode* newNode = createLLNode(element);

    while (node -> next != NULL) {
        node = node->next;
    }

    node->next = newNode;
    newNode->next = NULL;
}

LLNode *getElement(LLNode *node, int index) {
    // TODO: Implement if required
}