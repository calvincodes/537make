//
// Created by Arpit Jain on 11/1/18.
//

#ifndef INC_537MAKE_GRAPHNODE_H
#define INC_537MAKE_GRAPHNODE_H

#endif //INC_537MAKE_GRAPHNODE_H

#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H
#include "LinkedList.h"
#endif

#ifndef INC_537MAKE_CONSTANTS_H
#define INC_537MAKE_CONSTANTS_H
#include "constants.h"
#endif

typedef struct graphNode {

    char* element;
    LLNode* dependencies;
    LLNode* commands;
    struct graphNode** children;
} GraphNode;

GraphNode* createGraphNode(char *element, LLNode* dependencies, LLNode* commands);