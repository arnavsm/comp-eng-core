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
    int ret;
 
    // Create FIFO if it doesn't exist
    if (mkfifo(FIFO_NAME, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
 
    // Open FIFO for writing
    fd = open(FIFO_NAME, O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
 
    while (1) {
        printf("Writer (type a message): ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
 
            // Write to FIFO
            ret = write(fd, buffer, strlen(buffer) + 1);
            if (ret < 0) {
                if (errno == EAGAIN) {
                    printf("FIFO is full, try again later...\n");
                } else {
                    perror("write");
                    break;
                }
            } else {
                printf("Message written to FIFO.\n");
            }
 
            // Exit if the user types "exit"
            if (strcmp(buffer, "exit") == 0) {
                break;
            }
        }
    }
 
    close(fd);
    return 0;
}
