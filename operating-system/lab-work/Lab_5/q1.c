#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO1 "fifo_1"
#define FIFO2 "fifo_2"
#define BUF_SIZE 256

void writer() {
    int fd_write;
    char buffer[BUF_SIZE];
    
    fd_write = open(FIFO1, O_WRONLY);
    if (fd_write == -1) {
        perror("open FIFO1 for writing");
        exit(EXIT_FAILURE);
    }

    // while (1) {
        printf("Enter message: ");
        if (fgets(buffer, BUF_SIZE, stdin) != NULL) {
            ssize_t bytesWritten = write(fd_write, buffer, strlen(buffer));
            if (bytesWritten == -1) {
                perror("write to FIFO1");
                close(fd_write);
                exit(EXIT_FAILURE);
            }
        }
    // }

    close(fd_write);
}

void reader() {
    int fd_read;
    char buffer[BUF_SIZE];
    ssize_t bytesRead;
    
    fd_read = open(FIFO2, O_RDONLY);
    if (fd_read == -1) {
        perror("open FIFO2 for reading");
        exit(EXIT_FAILURE);
    }

    // while (1) {
        memset(buffer, 0, BUF_SIZE);
        bytesRead = read(fd_read, buffer, BUF_SIZE);
        if (bytesRead > 0) {
            printf("Received: %s", buffer);
        } else if (bytesRead == -1) {
            perror("read from FIFO2");
            close(fd_read);
            exit(EXIT_FAILURE);
        }
    // }

    close(fd_read);
}

int main() {

    int res_1 = mkfifo(FIFO1, 0777);
    int res_2 = mkfifo(FIFO2, 0777);
    
    if (res_1 == -1) {
        perror("mkfifo FIFO1");
        exit(EXIT_FAILURE);
    }
    
    if (res_2 == -1) {
        perror("mkfifo FIFO2");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        writer();
    } else {
        // Parent process
        reader();
    }

    return 0;
}

