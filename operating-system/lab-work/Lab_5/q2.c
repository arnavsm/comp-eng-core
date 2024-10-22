#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define FIFO_NAME "/tmp/myfifo"
#define BUF_SIZE 256

void writer() {
    int fd;
    char buffer[BUF_SIZE];
    int i;

    if (mkfifo(FIFO_NAME, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < 10; i++) {
        snprintf(buffer, BUF_SIZE, "Message %d\n", i);
        write(fd, buffer, strlen(buffer));
        printf("Written: %s", buffer);
        sleep(1);
    }

    close(fd);
}

void reader() {
    int fd;
    char buffer[BUF_SIZE];
    int res;

    fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(buffer, 0, BUF_SIZE);
        res = read(fd, buffer, BUF_SIZE);
        if (res > 0) {
            printf("Received: %s", buffer);
        } else if (res == -1 && errno != EAGAIN) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }

    close(fd);
}

int main() {

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        writer();
    } else {
        reader();
    }

    return 0;
}

