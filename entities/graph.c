//
// Created by Arpit Jain on 11/1/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "graph.h"

int main_() {

    linked_list_node* all_commands = createLLNode(
            "gcc -o prodcomm main.o reader.o munch1.o munch2.o writer.o constants.o queue.o struct_args.o -lpthread");
    graph_node* all = createGraphNode("all", NULL, all_commands);

    linked_list_node* main_o_commands = createLLNode("gcc -Wall -Wextra -c main.c");
    graph_node* main_o = createGraphNode("main_o", NULL, main_o_commands);

    linked_list_node* queue_o_commands = createLLNode("gcc -Wall -Wextra -c queue.c");
    graph_node* queue_o = createGraphNode("queue_o", NULL, queue_o_commands);

    all->children[0] = main_o;
    all->children[1] = queue_o;
    main_o->children = NULL;
    queue_o->children = NULL;

    printf("end");

//    graph_node* reader_o = (graph_node *) malloc(sizeof(graph_node));
//    graph_node* munch1_o = (graph_node *) malloc(sizeof(graph_node));
//    graph_node* munch2_o = (graph_node *) malloc(sizeof(graph_node));
//    graph_node* writer_o = (graph_node *) malloc(sizeof(graph_node));
//    graph_node* constants_o = (graph_node *) malloc(sizeof(graph_node));
//    graph_node* struct_args_o = (graph_node *) malloc(sizeof(graph_node));

//    linked_list_node* all_dependencies = createLLNode("main.o");
//    appendToLL(all_dependencies, "reader.o");
//    appendToLL(all_dependencies, "munch1.o");
//    appendToLL(all_dependencies, "munch2.o");
//    appendToLL(all_dependencies, "writer.o");
//    appendToLL(all_dependencies, "constants.o");
//    appendToLL(all_dependencies, "queue.o");
//    appendToLL(all_dependencies, "struct_args.o");


//    linked_list_node* main_o_dependencies = createLLNode("main.c");
//    appendToLL(main_o_dependencies, "reader.h");
//    appendToLL(main_o_dependencies, "munch1.h");
//    appendToLL(main_o_dependencies, "munch2.h");
//    appendToLL(main_o_dependencies, "writer.h");
//    appendToLL(main_o_dependencies, "queue.h");
//    appendToLL(main_o_dependencies, "struct_args.o");
//    appendToLL(main_o_dependencies, "constants.h");
}

graph_node* createGraphNode(char *element, linked_list_node* dependencies, linked_list_node* commands) {

    graph_node* graphNode = (graph_node *) malloc(sizeof(graph_node));

    if(!graphNode){
        fprintf(stderr, "Could not allocate memory for LinkedList data");
        exit(1);
    }
    graphNode->element = element;
    graphNode->dependencies = dependencies;
    graphNode->commands = commands;
    graphNode->children = (graph_node**) malloc(MAX_SIZE * sizeof(graphNode));
    return graphNode;
}

graph_node* createConnections(graph_node* graphArray[], unsigned int size){
    graph_node *root = NULL;

    for(int i = 0;i<size;i++){
        graphArray[i]->nodeNo = i;
    }
    for(int i = 0;i<size;i++){
        if(!root){
            root = graphArray[i];
        }
        linked_list_node *llNode = graphArray[i]->dependencies;
        int currChildCount = 0;
        while(llNode){

            for(int j =0;j<size;j++){
                if(strcmp(llNode->element, graphArray[j]->element) == 0){
                    graphArray[i]->children[currChildCount++] = graphArray[j];
                    break;
                }
            }
            llNode = llNode->next;
        }
    }
    return root;
}

int cyclic_util(int nodeNo, int visited[], int stack[], graph_node *graphNodeArray[], graph_node* node ){
    if(!visited[nodeNo]){
        visited[nodeNo] = 1;
        stack[nodeNo] = 1;
    }
    for(int i=0;i<MAX_SIZE && node->children[i] != NULL;i++){
        if (!visited[node->children[i]->nodeNo] && cyclic_util(node->children[i]->nodeNo, visited, stack, graphNodeArray, graphNodeArray[nodeNo]->children[i]))
            return 1;
        else if (stack[node->children[i]->nodeNo])
            return 1;
    }
    stack[node->nodeNo] = 0;
    return 0;
}

int is_cycle_found(unsigned int graphSize, graph_node *graphNodeArray[]){

    int visited[graphSize];
    int stack[graphSize];

    for(int i=0;i<graphSize;i++ ){
        visited[i] = 0;
        stack[i] = 0;
    }

    for(int i=0;i<graphSize;i++){
        if(cyclic_util(graphNodeArray[i]->nodeNo, visited, stack, graphNodeArray, graphNodeArray[i])){
            return 1;
        }
    }
    return 0;
}

