#include <stdio.h>
#include <unistd.h>

#define MAX_PROCESSES 7

int processTree[MAX_PROCESSES];
int pid;

void printTree(int index, int level) {
    if (index >= MAX_PROCESSES) return;
    printTree(2*index+2, level+1);
    for (int i = 0; i < level; i++) printf("  ");
    printf("Process %d\n", processTree[index]);
    printTree(2*index+1, level+1);
}

int main() {
    printf("Here I am just before first forking statement\n");
    pid = fork();
    processTree[0] = getpid();
    if (pid == 0) {
        printf("Here I am just after first forking statement\n");
        pid = fork();
        processTree[1] = getpid();
        if (pid == 0) {
            printf("Here I am just after second forking statement\n");
            pid = fork();
            processTree[3] = getpid();
            if (pid != 0) {
                processTree[4] = getpid();
            }
        } else {
            processTree[2] = getpid();
            pid = fork();
            processTree[5] = getpid();
            if (pid != 0) {
                processTree[6] = getpid();
            }
        }
    }
    printf("Hello World from process %d!\n", getpid());
    printTree(0, 0);
    return 0;
}
