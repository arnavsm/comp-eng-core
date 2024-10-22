#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define FIFO_NAME "myfifo"
#define BUF_SIZE 256
#define NUM_CHILDREN 10

void child_process(int id) {
    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    char buffer[BUF_SIZE];
    snprintf(buffer, BUF_SIZE, "Message from child %d\n", id);
    if (write(fd, buffer, strlen(buffer)) == -1) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}

int main() {
    // Create the FIFO if it does not exist
    if (mkfifo(FIFO_NAME, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Fork child processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            child_process(i + 1);
        }
    }

    // Close the write end of FIFO in the parent
    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
    }

    // Open FIFO for reading
    int fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    char buffer[BUF_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, BUF_SIZE - 1)) > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("Received: %s", buffer);
    }

    if (bytesRead == -1) {
        perror("read");
    }

    close(fd);

    // Remove the FIFO
    if (unlink(FIFO_NAME) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}

