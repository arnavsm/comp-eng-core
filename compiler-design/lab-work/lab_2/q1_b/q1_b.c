#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


int main() {
    int state = 0;
    char str[100];
    printf("Enter sequence: ");
    gets(str);
    int n = strlen(str);
    for (int i = 0; i < n; i++) {
        char current = str[i];
        switch(state) {
            case 0: 
                if (current == '1') state = 1;
                else if (current == '0') state = 3;
                else return 1;
                break;
            case 1: 
                if (current == '1') state = 0;
                else if (current == '0') state = 2;
                else return 1;
                break;
            case 2: 
                if (current == '1') state = 3;
                else if (current == '0') state = 1;
                else return 1;
                break;
            case 3: 
                if (current == '1') state = 2;
                else if (current == '0') state = 0;
                else return 1;
                break;
            default: return 1;
        }
    }
    if (state == 1) printf("Correct!\n");
    else printf("Wrong\n");
    return 0;
    
}