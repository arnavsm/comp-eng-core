#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
 
#define FIFO_NAME "/tmp/fifo_example"
 
int main() {
    int fd;
    char buffer[100];
    ssize_t n;
 
    // Create FIFO if it doesn't exist
    if (mkfifo(FIFO_NAME, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
 
    // Open FIFO for reading
    fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
 
    while (1) {
        // Read from FIFO
        n = read(fd, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Message read from FIFO: %s\n", buffer);
        } else if (n < 0) {
            if (errno == EAGAIN) {
                printf("No data available, try again later...\n");
            } else {
                perror("read");
                break;
            }
        }
 
        sleep(1); // Sleep for 1 second to avoid busy-waiting
    }
 
    close(fd);
    return 0;
}
