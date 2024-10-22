#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int i; for (i=0; i < 3; i++) {
	fork();
	printf("hello\n");
	}
	return 0;
}
