#include <stdio.h>
#include <unistd.h>

int main() 
{
	char buff[20];
	read(0,buff,20);
	for (int i = 19; i >= 0; i--)
	{
		printf("%c", buff[i]);
	}
	
	return 0;
}
