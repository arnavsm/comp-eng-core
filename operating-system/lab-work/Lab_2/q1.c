#include <stdio.h>
#include <unistd.h>

int main() 
{
	int byte_size = write(1, "hello_arnav\n", 12);
	printf("\n%d\n", byte_size);
	return 0;
}
