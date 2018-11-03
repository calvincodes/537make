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

    //TODO: Change it to target name
    char* element;
    int nodeNo;
    LLNode* dependencies;
    LLNode* commands;
    struct graphNode** children;
} GraphNode;

GraphNode* createGraphNode(char *element, LLNode* dependencies, LLNode* commands);

GraphNode* createConnections(GraphNode* graphArray[], unsigned int size);
int is_cycle_found(unsigned int size, GraphNode *graphNodeArray[]);