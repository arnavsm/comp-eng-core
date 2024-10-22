#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("seeking.txt", O_RDONLY);
    dup2(fd, 5);
    close(fd);
    char buffer[1024];
    read(5, buffer, 1024);
    printf("Read from duplicated fd: %s\n", buffer);
    close(5);
    return 0;
}
