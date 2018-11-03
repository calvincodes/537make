//
// Created by Arpit Jain on 11/1/18.
//

#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <printf.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include "Traversal.h"

/**
 * Return ture if lhs is less than rhs
 * @param lhs
 * @param rhs
 * @return
 */
bool isLHSLessThanRHS(struct timespec lhs, struct timespec rhs) {

//    printf("LHS lmd: %lld.%.9ld\n", (long long)lhs.tv_sec, lhs.tv_nsec);
//    printf("RHS lmd: %lld.%.9ld\n", (long long)rhs.tv_sec, rhs.tv_nsec);

    if (lhs.tv_sec == rhs.tv_sec)
        return lhs.tv_nsec < rhs.tv_nsec;
    else
        return lhs.tv_sec < rhs.tv_sec;
}


bool commandExecutionRequired(GraphNode* root) {

    if(root->dependencies != NULL) {

        struct stat targetStat;
        FILE *targetPointer = fopen(root->element, "r");
        fstat(fileno(targetPointer), &targetStat);
        fclose(targetPointer);
        struct timespec targetLmd = targetStat.st_mtim;

        LLNode* dependencies = root->dependencies;
        while(dependencies) {
            printf("Dependency: %s\n", dependencies->element);
            struct stat dependencyStat;
            FILE *dependencyPointer = fopen(dependencies->element, "r");
            fstat(fileno(dependencyPointer), &dependencyStat);
            fclose(dependencyPointer);
            struct timespec dependencyLmd = dependencyStat.st_mtim;

            if(isLHSLessThanRHS(targetLmd, dependencyLmd)) {
                return true;
            }

            dependencies = dependencies->next;
        }

        return false;
    }

    return true;
}

void executeNodeCommands(GraphNode* root) {

    if(commandExecutionRequired(root)) {
        LLNode* temphead = root->commands;
        while (temphead != NULL) {
            pid_t parent = getpid();
            pid_t pid = fork();
            if (pid == -1) { // Error, failed to fork()
                printf("Failed to fork(). Terminating at once.");
                _exit(EXIT_FAILURE);
            }

            else if (pid > 0) { // Parent process
                int status;
                waitpid(pid, &status, 0);
                if (status != EXIT_SUCCESS) {
                    printf("Failed to execute command. Error status %d", status);
                    _exit(EXIT_FAILURE);
                }
                temphead = temphead->next;
            }

            else if (pid == 0) { // Child process

                char copiedCmd[MAX_SIZE];
                strncpy(copiedCmd, temphead->element, MAX_SIZE);
                char *argv[MAX_SIZE];
                int i = 0;
                char *split = strtok(copiedCmd, " ");
                while (split) {
                    argv[i] = split;
                    i++;
                    split = strtok(NULL, " ");
                }
                argv[i] = NULL;
                char *cmd = argv[0];

                execvp(cmd, argv);
                // The exec() functions only return if an error has occurred.
                // The return value is -1, and errno is set to indicate the error.

                _exit(EXIT_FAILURE);
            }

            else { // IMPOSSIBLE ZONE
                printf("PID can not be negative. Terminating at once.");
                _exit(EXIT_SUCCESS);
            }
        }
    }
}

void traverseAndExecute(GraphNode* root) {

    if (root->children == NULL) {
        executeNodeCommands(root);
        return;
    }

    for (int i = 0; i < MAX_SIZE; ++i) {
        if (root->children[i] == NULL) {
            break;
        }
        traverseAndExecute(root->children[i]);
    }

    executeNodeCommands(root);
}

int main() {

    LLNode* all_dependencies = createLLNode("temp.c");
    appendToLL(all_dependencies, "temp1.c");
    LLNode* all_commands = createLLNode("gcc -c temp.c temp1.c temp2.c");
    appendToLL(all_commands, "gcc -c main.c");
    appendToLL(all_commands, "echo all success....");
    GraphNode* all = createGraphNode("temp2.c", all_dependencies, all_commands);

    LLNode* a_commands = createLLNode("gcc -c main.c");
    appendToLL(a_commands, "echo a success....");
    GraphNode* a = createGraphNode("main.c", NULL, a_commands);
    all->children[0] = a;
    a->children = NULL;

    LLNode* b_commands = createLLNode("gcc -c main.c");
    appendToLL(b_commands, "echo b success....");
    GraphNode* b = createGraphNode("temp.c", NULL, b_commands);
    all->children[1] = b;

    LLNode* c_commands = createLLNode("gcc -c main.c");
    appendToLL(c_commands, "echo c success....");
    GraphNode* c = createGraphNode("temp1.c", NULL, c_commands);
    b->children[0] = c;
    c->children = NULL;

    LLNode* d_commands = createLLNode("gcc -c main.c");
    appendToLL(d_commands, "echo d success....");
    GraphNode* d = createGraphNode("temp2.c", NULL, d_commands);
    all->children[2] = d;

    LLNode* e_commands = createLLNode("gcc -c main.c");
    appendToLL(e_commands, "echo e success....");
    GraphNode* e = createGraphNode("temp.c", NULL, e_commands);
    d->children[0] = e;
    e->children = NULL;

    LLNode* f_commands = createLLNode("gcc -c main.c");
    appendToLL(f_commands, "echo f success....");
    GraphNode* f = createGraphNode("main.c", NULL, f_commands);
    d->children[1] = f;
    f->children = NULL;

    traverseAndExecute(all);
}