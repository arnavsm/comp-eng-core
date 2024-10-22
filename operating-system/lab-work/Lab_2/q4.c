#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() 
{
	char buff[256];
	int fd1 = open("test.txt", O_RDONLY);
	read(fd1, buff, 256);
	int fd2 = open("towrite.txt", O_CREAT | O_RDWR);
	write(fd2, buff, 256);
	
	return 0;
}
