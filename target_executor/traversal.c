
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