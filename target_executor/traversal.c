
// Created by Arpit Jain on 11/1/18.
//

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <printf.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include "traversal.h"

/**
 * Return ture if lhs is less than rhs
 * @param lhs
 * @param rhs
 * @return
 */

//bool isLHSLessThanRHS(struct timespec lhs, struct timespec rhs) {
//
////    printf("LHS lmd: %lld.%.9ld\n", (long long)lhs.tv_sec, lhs.tv_nsec);
////    printf("RHS lmd: %lld.%.9ld\n", (long long)rhs.tv_sec, rhs.tv_nsec);
//
//    if (lhs.tv_sec == rhs.tv_sec)
//        return lhs.tv_nsec < rhs.tv_nsec;
//    else
//        return lhs.tv_sec < rhs.tv_sec;
//}
//
//
//bool commandExecutionRequired(graph_node* root) {
//
//    if(root->dependencies != NULL) {
//
//        struct stat targetStat;
//        FILE *targetPointer = fopen(root->element, "r");
//
//        if (!targetPointer) {
//            // Target file does not exists
//            return true;
//        } else {
//
//            fstat(fileno(targetPointer), &targetStat);
//            fclose(targetPointer);
//            struct timespec targetLmd = targetStat.st_mtim;
//
//            linked_list_node* dependencies = root->dependencies;
//            while(dependencies) {
//                struct stat dependencyStat;
//                FILE *dependencyPointer = fopen(dependencies->element, "r");
//
//                if (!dependencyPointer) {
//                    // Dependency file does not exists - THROW ERROR
//                    printf("Dependency file '%s' does NOT exists", dependencies->element);
//                    exit(EXIT_FAILURE);
//                } else {
//                    fstat(fileno(dependencyPointer), &dependencyStat);
//                    fclose(dependencyPointer);
//                    struct timespec dependencyLmd = dependencyStat.st_mtim;
//
//                    if(isLHSLessThanRHS(targetLmd, dependencyLmd)) {
//                        return true;
//                    }
//                }
//
//                dependencies = dependencies->next;
//            }
//
//            return false;
//        }
//    }
//
//    return true;
//}
//
//void executeNodeCommands(graph_node* root) {
//
//    if(commandExecutionRequired(root)) {
//        linked_list_node* temphead = root->commands;
//        while (temphead != NULL) {
//            pid_t parent = getpid();
//            pid_t pid = fork();
//            if (pid == -1) { // Error, failed to fork()
//                printf("Failed to fork(). Terminating at once.");
//                exit(EXIT_FAILURE);
//            }
//
//            else if (pid > 0) { // Parent process
//                int status;
//                waitpid(pid, &status, 0);
//                if (status != EXIT_SUCCESS) {
//                    printf("Failed to execute command. Error status %d", status);
//                    exit(EXIT_FAILURE);
//                }
//                temphead = temphead->next;
//            }
//
//            else if (pid == 0) { // Child process
//
//                char copiedCmd[MAX_SIZE];
//                strncpy(copiedCmd, temphead->element, MAX_SIZE);
//                char *argv[MAX_SIZE];
//                int i = 0;
//                char *split = strtok(copiedCmd, " ");
//                while (split) {
//                    argv[i] = split;
//                    i++;
//                    split = strtok(NULL, " ");
//                }
//                argv[i] = NULL;
//                char *cmd = argv[0];
//
//                execvp(cmd, argv);
//                // The exec() functions only return if an error has occurred.
//                // The return value is -1, and errno is set to indicate the error.
//
//                exit(EXIT_FAILURE);
//            }
//
//            else { // IMPOSSIBLE ZONE
//                printf("PID can not be negative. Terminating at once.");
//                _exit(EXIT_SUCCESS);
//            }
//        }
//    }
//}

void traverseAndExecute(graph_node* root) {

    if (root->children[0] == NULL) {
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


int main_5() {

    linked_list_node* all_dependencies = createLLNode("temp.c");
    appendToLL(all_dependencies, "temp1.c");
    linked_list_node* all_commands = createLLNode("gcc -c temp.c temp1.c temp2.c");
    appendToLL(all_commands, "gcc -c main.c");
    appendToLL(all_commands, "echo all success....");
    graph_node* all = createGraphNode("temp2.c", all_dependencies, all_commands);

    linked_list_node* a_commands = createLLNode("gcc -c main.c");
    appendToLL(a_commands, "echo a success....");
    graph_node* a = createGraphNode("main.c", NULL, a_commands);
    all->children[0] = a;
    a->children = NULL;

    linked_list_node* b_commands = createLLNode("gcc -c main.c");
    appendToLL(b_commands, "echo b success....");
    graph_node* b = createGraphNode("temp.c", NULL, b_commands);
    all->children[1] = b;

    linked_list_node* c_commands = createLLNode("gcc -c main.c");
    appendToLL(c_commands, "echo c success....");
    graph_node* c = createGraphNode("temp1.c", NULL, c_commands);
    b->children[0] = c;
    c->children = NULL;

    linked_list_node* d_commands = createLLNode("gcc -c main.c");
    appendToLL(d_commands, "echo d success....");
    graph_node* d = createGraphNode("temp2.c", NULL, d_commands);
    all->children[2] = d;

    linked_list_node* e_commands = createLLNode("gcc -c main.c");
    appendToLL(e_commands, "echo e success....");
    graph_node* e = createGraphNode("temp.c", NULL, e_commands);
    d->children[0] = e;
    e->children = NULL;

    linked_list_node* f_commands = createLLNode("gcc -c main.c");
    appendToLL(f_commands, "echo f success....");
    graph_node* f = createGraphNode("main.c", NULL, f_commands);
    d->children[1] = f;
    f->children = NULL;

    traverseAndExecute(all);
}