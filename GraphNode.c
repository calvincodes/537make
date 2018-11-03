//
// Created by Arpit Jain on 11/1/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "GraphNode.h"

int main_() {

    LLNode* all_commands = createLLNode(
            "gcc -o prodcomm main.o reader.o munch1.o munch2.o writer.o constants.o queue.o struct_args.o -lpthread");
    GraphNode* all = createGraphNode("all", NULL, all_commands);

    LLNode* main_o_commands = createLLNode("gcc -Wall -Wextra -c main.c");
    GraphNode* main_o = createGraphNode("main_o", NULL, main_o_commands);

    LLNode* queue_o_commands = createLLNode("gcc -Wall -Wextra -c queue.c");
    GraphNode* queue_o = createGraphNode("queue_o", NULL, queue_o_commands);

    all->children[0] = main_o;
    all->children[1] = queue_o;
    main_o->children = NULL;
    queue_o->children = NULL;

    printf("end");

//    GraphNode* reader_o = (GraphNode *) malloc(sizeof(GraphNode));
//    GraphNode* munch1_o = (GraphNode *) malloc(sizeof(GraphNode));
//    GraphNode* munch2_o = (GraphNode *) malloc(sizeof(GraphNode));
//    GraphNode* writer_o = (GraphNode *) malloc(sizeof(GraphNode));
//    GraphNode* constants_o = (GraphNode *) malloc(sizeof(GraphNode));
//    GraphNode* struct_args_o = (GraphNode *) malloc(sizeof(GraphNode));

//    LLNode* all_dependencies = createLLNode("main.o");
//    appendToLL(all_dependencies, "reader.o");
//    appendToLL(all_dependencies, "munch1.o");
//    appendToLL(all_dependencies, "munch2.o");
//    appendToLL(all_dependencies, "writer.o");
//    appendToLL(all_dependencies, "constants.o");
//    appendToLL(all_dependencies, "queue.o");
//    appendToLL(all_dependencies, "struct_args.o");


//    LLNode* main_o_dependencies = createLLNode("main.c");
//    appendToLL(main_o_dependencies, "reader.h");
//    appendToLL(main_o_dependencies, "munch1.h");
//    appendToLL(main_o_dependencies, "munch2.h");
//    appendToLL(main_o_dependencies, "writer.h");
//    appendToLL(main_o_dependencies, "queue.h");
//    appendToLL(main_o_dependencies, "struct_args.o");
//    appendToLL(main_o_dependencies, "constants.h");
}

GraphNode* createGraphNode(char *element, LLNode* dependencies, LLNode* commands) {

    GraphNode* graphNode = (GraphNode *) malloc(sizeof(GraphNode));

    if(!graphNode){
        fprintf(stderr, "Could not allocate memory for LinkedList data");
        exit(1);
    }
    graphNode->element = element;
    graphNode->dependencies = dependencies;
    graphNode->commands = commands;
    graphNode->children = (GraphNode**) malloc(MAX_SIZE * sizeof(graphNode));
    return graphNode;
}

GraphNode* createConnections(GraphNode* graphArray[], unsigned int size){
    GraphNode *root = NULL;

    for(int i = 0;i<size;i++){
        graphArray[i]->nodeNo = i;
    }
    for(int i = 0;i<size;i++){
        if(!root){
            root = graphArray[i];
        }
        LLNode *llNode = graphArray[i]->dependencies;
        while(llNode){
            int currChildCount = 0;
            for(int j =0;j<size;j++){
                printf("%s\n", llNode->element);
                if(strcmp(llNode->element, graphArray[j]->element) == 0){
                    graphArray[i]->children[currChildCount++] = graphArray[j];
                }
            }
            llNode = llNode->next;
        }
    }
    return root;
}

int cyclic_util(int nodeNo, int visited[], int stack[], GraphNode *graphNodeArray[], GraphNode* node ){
    if(!visited[nodeNo]){
        visited[nodeNo] = 1;
        stack[nodeNo] = 1;
    }
    for(int i=0;i<MAX_SIZE && node->children[i] != NULL;i++){
        if (!visited[node->children[i]->nodeNo] && cyclic_util(node->children[i]->nodeNo, visited, stack, graphNodeArray, graphNodeArray[nodeNo]))
            return 1;
        else if (stack[node->children[i]->nodeNo])
            return 1;
    }
    stack[node->nodeNo] = 0;
    return 0;
}

int is_cycle_found(unsigned int size, GraphNode *graphNodeArray[]){

    int visited[size];
    int stack[size];

    for(int i=0;i<size;i++ ){
        visited[i] = 0;
        stack[i] = 0;
    }

    for(int i=0;i<size;i++){
        if(cyclic_util(graphNodeArray[i]->nodeNo, visited, stack, graphNodeArray, graphNodeArray[i])){
            return 1;
        }
    }
    return 0;
}

