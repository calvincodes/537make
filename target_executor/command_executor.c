//
// Created by Arpit Jain on 11/3/18.
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
#include "command_executor.h"

bool isLHSLessThanRHS(struct timespec lhs, struct timespec rhs) {

//    printf("LHS lmd: %lld.%.9ld\n", (long long)lhs.tv_sec, lhs.tv_nsec);
//    printf("RHS lmd: %lld.%.9ld\n", (long long)rhs.tv_sec, rhs.tv_nsec);

    if (lhs.tv_sec == rhs.tv_sec)
        return lhs.tv_nsec < rhs.tv_nsec;
    else
        return lhs.tv_sec < rhs.tv_sec;
}


bool commandExecutionRequired(graph_node* root) {

    if(root->dependencies != NULL) {

        struct stat targetStat;
        FILE *targetPointer = fopen(root->element, "r");

        if (!targetPointer) {
            // Target file does not exists
            return true;
        } else {

            fstat(fileno(targetPointer), &targetStat);
            fclose(targetPointer);
            struct timespec targetLmd = targetStat.st_mtim;

            linked_list_node* dependencies = root->dependencies;
            while(dependencies) {
                struct stat dependencyStat;
                FILE *dependencyPointer = fopen(dependencies->element, "r");

                if (!dependencyPointer) {
                    // Dependency file does not exists - THROW ERROR
                    printf("Dependency file '%s' does NOT exists", dependencies->element);
                    exit(EXIT_FAILURE);
                } else {
                    fstat(fileno(dependencyPointer), &dependencyStat);
                    fclose(dependencyPointer);
                    struct timespec dependencyLmd = dependencyStat.st_mtim;

                    if(isLHSLessThanRHS(targetLmd, dependencyLmd)) {
                        return true;
                    }
                }

                dependencies = dependencies->next;
            }

            return false;
        }
    }

    return true;
}

void executeNodeCommands(graph_node* root) {

    if(commandExecutionRequired(root)) {
        linked_list_node* temphead = root->commands;
        while (temphead != NULL) {
            pid_t pid = fork();
            if (pid == -1) { // Error, failed to fork()
                printf("Failed to fork(). Terminating at once.");
                exit(EXIT_FAILURE);
            }

            else if (pid > 0) { // Parent process
                int status;
                waitpid(pid, &status, 0);
                if (status != EXIT_SUCCESS) {
                    printf("Failed to execute command. Error status %d", status);
                    exit(EXIT_FAILURE);
                }
                temphead = temphead->next;
            }

            else if (pid == 0) { // Child process

                bool inputRedirection = false;
                bool outputRedirection = false;
                char* file_name;

                char copiedCmd[MAX_SIZE];
                strncpy(copiedCmd, temphead->element, MAX_SIZE);
                char *argv[MAX_SIZE];
                int i = 0;
                char *split = strtok(copiedCmd, " ");
                while (split) {

                    if (strcmp(split, "<") == 0) {
                        inputRedirection = true;
                        file_name = strtok(NULL, " ");
                        i++;
                        break;
                    } else if (strcmp(split, ">") == 0) {
                        outputRedirection = true;
                        file_name = strtok(NULL, " ");
                        i++;
                        break;
                    } else {
                        argv[i] = split;
                        i++;
                        split = strtok(NULL, " ");
                    }
                }
                argv[i] = NULL;
                char *cmd = argv[0];

                if (inputRedirection && outputRedirection) {
                    // TODO confirm and throw error.
                }

                if (inputRedirection) {
//                    close(STDIN_FILENO);
                    int in = open(file_name, O_RDONLY);
                    // replace standard input with input file

                    dup2(in, STDIN_FILENO);
                    // close unused file descriptors

                    close(in);
                }

                if (outputRedirection) {
//                    close(STDOUT_FILENO);
                    int out = open(file_name, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
                    // replace standard output with output file

                    dup2(out, STDOUT_FILENO);
                    // close unused file descriptors

                    close(out);
                }

                execvp(cmd, argv);
                // The exec() functions only return if an error has occurred.
                // The return value is -1, and errno is set to indicate the error.

                exit(EXIT_FAILURE);
            }

            else { // IMPOSSIBLE ZONE
                printf("PID can not be negative. Terminating at once.");
                _exit(EXIT_SUCCESS);
            }
        }
    }
}
