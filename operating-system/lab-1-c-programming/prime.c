#include <stdio.h>

int main() {
	int num;
	printf("Enter Number: ");
	scanf("%d", &num);
	 
	for (int i = 2; i < num; i++) {
		if (num % i == 0) {
			printf("Not Prime\n");
			return 0;
		}
	}
	printf("Prime\n");
	return 0;
}
