#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == 0) {
        for (int i = 1; i <= 10; i++) {
            printf("P1: %d\n", i);
            sleep(1);
        }
        return 0;
    }

    pid2 = fork();
    if (pid2 == 0) {
        for (char c = 'A'; c <= 'Z'; c++) {
            printf("P2: %c\n", c);
            sleep(1);
        }
        return 0;
    }

    printf("Parent process waiting for children to finish...\n");
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    printf("Parent process finished.\n");

    return 0;
}
