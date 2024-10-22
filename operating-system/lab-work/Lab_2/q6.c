#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("seeking.txt", O_RDONLY);
    int dup_fd = dup(fd);
    close(fd);
    char buffer[1024];
    read(dup_fd, buffer, 1024);
    printf("Read from duplicated fd: %s\n", buffer);
    close(dup_fd);
    return 0;
}
