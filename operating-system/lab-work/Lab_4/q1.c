#include <stdio.h>
#include <stdlib.h>
 
int main() {
    FILE *fp;
    char buffer[256];
    char user_input[256];
 
    fp = popen("./input", "r");
    if (fp == NULL) {
        perror("Failed to run input program");
        return 1;
    }
 
    printf("Output from 'input' program:\n");
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
 
    pclose(fp);
 
    printf("\nPlease enter additional data:\n");
    fgets(user_input, sizeof(user_input), stdin);
 
    printf("You entered:\n%s", user_input);
 
    return 0;
}
