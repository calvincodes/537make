
//
// Created by Arpit Jain on 11/1/18.
//

#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H

#endif //PROJECT_NODE_H

typedef struct llnode {
    char* element;
    struct llnode* next;
} LLNode;

LLNode *createLLNode(char *element);

void appendToLL(LLNode *node, char *element);

LLNode *getElement(LLNode *node, int index);