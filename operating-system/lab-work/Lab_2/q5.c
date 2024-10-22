#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    char buff[11]; 
    int fd1 = open("seeking.txt", O_RDONLY);
    if (fd1 == -1) {
        perror("open");
        return 1;
    }

    read(fd1, buff, 10);
    buff[10] = '\0';
    printf("First 10 characters: %s\n", buff);

    read(fd1, buff, 10);
    lseek(fd1,10,SEEK_CUR);
    
    buff[10] = '\0';
    printf("Next 10 characters: %s\n", buff);

    close(fd1);
    return 0;
}
