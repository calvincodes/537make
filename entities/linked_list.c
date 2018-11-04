//
// Created by Arpit Jain on 11/1/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"
#include "../utils/constants.h"

int main_1() {
    linked_list_node* head = createLLNode("arpit");
    appendToLL(head, "jain");
    appendToLL(head, "uwm");
    appendToLL(head, "intern");
    appendToLL(head, "lagwa");
    appendToLL(head, "do");
    printf("end");
}

linked_list_node *createLLNode(char *element) {

    linked_list_node* llNode = (linked_list_node *) malloc(sizeof(linked_list_node));

    if(!llNode){
        fprintf(stderr, "Could not allocate memory for LinkedList data");
        exit(EXIT_FAILURE);
    }

    llNode->element = malloc(MAX_SIZE * sizeof(char*));
    strcpy(llNode->element, element);
    llNode->next = NULL;
    return llNode;
}

void appendToLL(linked_list_node *node, char *element) {

    if (node == NULL) {
        fprintf(stderr, "Null node passed to appendToLL function");
    }

    linked_list_node* newNode = createLLNode(element);

    linked_list_node* tempHead = node;
    while (tempHead -> next != NULL) {
        tempHead = tempHead->next;
    }

    tempHead->next = newNode;
    newNode->next = NULL;
}

linked_list_node *getElement(linked_list_node *node, int index) {
    // TODO: Implement if required
}