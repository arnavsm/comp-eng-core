#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main()
{
	FILE *rd;
	char buffer[50];
	rd=popen("ls","r"); //pipe opened in reading mode
	fread(buffer, 1, 40, rd); //will read only 50 characters
	printf("%s\n", buffer);
	pclose(rd);
}
