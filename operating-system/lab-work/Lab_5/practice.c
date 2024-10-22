#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	int res = mkfifo("fifo1", 0777); // named-pipe with name fifo1
	printf("Named Pipe Created\n");
	return 0;
}
