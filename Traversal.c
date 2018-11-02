//
// Created by Arpit Jain on 11/1/18.
//

#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <printf.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "Traversal.h"

void executeNodeCommands(LLNode* commands) {

    LLNode* temphead = commands;
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

//            char *cmd = "echo";
//            char* argv[3];
//            argv[0] = "echo";
//            argv[1] = temphead->element;
//            argv[2] = (char *)NULL;

            char *cmd = "gcc";
            char *argv[4];
            argv[0] = "gcc";
            argv[1] = "-c";
            argv[2] = "main.c";
            argv[3] = NULL;

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

void traverseAndExecute(GraphNode* root) {

    if (root->children == NULL) {
        executeNodeCommands(root->commands);
        return;
    }

    for (int i = 0; i < MAX_SIZE; ++i) {
        if (root->children[i] == NULL) {
            break;
        }
        traverseAndExecute(root->children[i]);
    }

    executeNodeCommands(root->commands);
}

int main() {

    LLNode* all_commands = createLLNode("gcc -c all_1");
    appendToLL(all_commands, "gcc -c all_2");
    appendToLL(all_commands, "gcc -c all_3");
    GraphNode* all = createGraphNode("all", NULL, all_commands);

    LLNode* a_commands = createLLNode("gcc -c a_1");
    GraphNode* a = createGraphNode("a", NULL, a_commands);
    all->children[0] = a;
    a->children = NULL;

    LLNode* b_commands = createLLNode("gcc -c b_1");
    GraphNode* b = createGraphNode("b", NULL, b_commands);
    all->children[1] = b;

    LLNode* c_commands = createLLNode("gcc -c c_1");
    GraphNode* c = createGraphNode("c", NULL, c_commands);
    b->children[0] = c;
    c->children = NULL;

    LLNode* d_commands = createLLNode("gcc -c d_1");
    GraphNode* d = createGraphNode("d", NULL, d_commands);
    all->children[2] = d;

    LLNode* e_commands = createLLNode("gcc -c e_1");
    GraphNode* e = createGraphNode("e", NULL, e_commands);
    d->children[0] = e;
    e->children = NULL;

    LLNode* f_commands = createLLNode("gcc -c f_1");
    GraphNode* f = createGraphNode("f", NULL, f_commands);
    d->children[1] = f;
    f->children = NULL;

    traverseAndExecute(all);
}