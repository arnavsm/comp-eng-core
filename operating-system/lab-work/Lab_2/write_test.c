#include <stdio.h>
#include <unistd.h>

int main() 
{
	int size = write(1, "hello\n", 3);
	printf("\n%d\n", size);
	return 0;
}
