#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 100
#define TERMINATION_MESSAGE "exit"

int main() {
    int fd_pc[2];
    int fd_cp[2];
    pid_t p;
    char buffer[BUFFER_SIZE];

    if (pipe(fd_pc) == -1 || pipe(fd_cp) == -1) {
        perror("pipe");
        return 1;
    }

    p = fork();
    if (p == -1) {
        perror("fork");
        return 1;
    }

    if (p > 0) {  
        close(fd_pc[0]);  
        close(fd_cp[1]);

        while (1) {
            printf("Parent: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            write(fd_pc[1], buffer, strlen(buffer));

            if (strstr(buffer, TERMINATION_MESSAGE) != NULL) {
                break;
            }

            int n = read(fd_cp[0], buffer, BUFFER_SIZE);
            buffer[n] = '\0';
            printf("Parent received: %s", buffer);

            if (strstr(buffer, TERMINATION_MESSAGE) != NULL) {
                break;
            }
        }

        close(fd_pc[1]);
        close(fd_cp[0]);

        wait(NULL);

    } else {
        close(fd_pc[1]);
        close(fd_cp[0]);

        while (1) {
          
            int n = read(fd_pc[0], buffer, BUFFER_SIZE);
            buffer[n] = '\0';
            printf("Child received: %s", buffer);

            if (strstr(buffer, TERMINATION_MESSAGE) != NULL) {
                break;
            }

            printf("Child: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            write(fd_cp[1], buffer, strlen(buffer));

            if (strstr(buffer, TERMINATION_MESSAGE) != NULL) {
                break;
            }
        }

        close(fd_pc[0]);
        close(fd_cp[1]);

        _exit(0);
    }

    return 0;
}

