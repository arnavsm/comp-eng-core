#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() 
{
	char buff[20];
	int fd1 = open("test.txt", O_RDONLY);
	read(fd1, buff, 20);
	int fd2 = open("towrite.txt", O_CREAT | O_RDWR);
	write(fd2, buff,10);
	
	return 0;
}
