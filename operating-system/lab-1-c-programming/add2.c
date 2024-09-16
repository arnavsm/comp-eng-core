#include <stdio.h>

int main() {
    int a;
    int b;
    
    printf("Enter First Number: ");
    scanf("%d", &a);
    printf("Enter Second Number: ");    
    scanf("%d", &b);
    
    a += b;
    printf("%d is the sum.\n", a);
    return 0;
}

