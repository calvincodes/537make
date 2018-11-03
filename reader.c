//
// Created by Anshu on 11/2/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"

void reader() {
    FILE *file_pointer;
    char *line = (char *) malloc(MAX_SIZE * sizeof(char));

    if (line == NULL) {
        fprintf(stderr, "Unable to allocate memory for reader input buffer");
        exit(1);
    }

    size_t len = 0;
    GraphNode *graphNodeArray[MAX_SIZE];

    // Reading Makefile
    file_pointer = fopen("testmake", "r");
    if (!file_pointer) {
        // In case readfile is not present. Trying reading Makefile
        file_pointer = fopen("akefile", "r");
        // If this also fails then throw error
        if (!file_pointer) {
            fprintf(stderr, "Could not find makefile or Makefile");
            exit(EXIT_FAILURE);
        }
    }
    int lineNo = 0;
    unsigned int curNode = 0;
    unsigned int index = 0;
    // Read line by line the contents of the file
    GraphNode *node = NULL;
    int c;
    do {
        lineNo++;


        // Concatente everything line;
        do {
            c = fgetc(file_pointer);
            line[index++] = (char) c;
        } while (c != '\n' && c != EOF && index < MAX_SIZE);

        if (index >= MAX_SIZE) {
            fprintf(stderr, "THIS LINE EXHAUSTED THE BUFFER SIZE %d AND HAS BEEN DISCARDED\n", MAX_SIZE);
            index = 0;
            free(line);
            line = (char *) malloc(MAX_SIZE * sizeof(char));
            continue;
        } else {
            line[index - 1] = '\0';
        }

        char *token;

        if (line[0] == '\t') {
            // Commands

            token = strtok(line, "\t");
            if (node == NULL) {
                fprintf(stderr, "Commands before target. Don't know what to do. exiting!!!");
                exit(EXIT_FAILURE);
            } else {
                if (!node->commands) {
                    LLNode *llNode = createLLNode(token);
                    node->commands = llNode;
                } else {
                    appendToLL(node->commands, token);
                }
            }

        } else if (line[0] == '#' || line[0] == '\0') {
            index = 0;
            free(line);
            line = (char *) malloc(MAX_SIZE * sizeof(char));
            continue;
        } else {
            // Now check if it's a target or not

            token = strtok(line, ":");
            // Line which is not comment, target or command
            if (!token) {
                fprintf(stderr, "%d: Invalid line : %s\n", lineNo, line);
                exit(EXIT_FAILURE);
            }

            char *targetName = malloc(sizeof(char) * MAX_SIZE);
            strcpy(targetName, token);

            token = strtok(NULL, " ");
            if (!token) {
                fprintf(stderr, "%d: Invalid line : %s\n", lineNo, line);
                exit(EXIT_FAILURE);
            }
            // New Target found. Create a new graph node.

            node = createGraphNode(targetName, NULL, NULL);
            graphNodeArray[curNode++] = node;

            int total_dep = 0;
            while (token != NULL) {
                if (strlen(token) > 0) {
                    total_dep++;
                    if (!node->dependencies) {
                        LLNode *llNode = createLLNode(token);
                        node->dependencies = llNode;
                    } else {
                        appendToLL(node->dependencies, token);
                    }
                }

                token = strtok(NULL, " ");
            }
            if (total_dep == 0) {
                fprintf(stderr, "%d: Invalid line : %s\n", lineNo, line);
                exit(EXIT_FAILURE);
            }
        }
        index = 0;
        free(line);
        line = (char *) malloc(MAX_SIZE * sizeof(char));

    } while (c != EOF);


    createConnections(graphNodeArray, curNode);
    int isCycleFound = is_cycle_found(curNode, graphNodeArray);
    printf("Dependency found %d", isCycleFound);

    GraphNode* root = graphNodeArray[0];
    int i = 0;

    while (root != NULL) {
        traverseAndExecute(root);
        i++;
        root = graphNodeArray[i];
    }

    if (fclose(file_pointer)) {
        fprintf(stderr, "Failed to close makefile or Makefile");
        exit(EXIT_FAILURE);
    }
}

int main() {
    reader();
    exit(EXIT_SUCCESS);
}
