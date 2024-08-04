#include <stdio.h>

int main() {
    char c;
    int state = 0;

    printf("Enter a string: ");
    while ((c = getchar()) != '\n' && c != EOF) {
        switch (state) {
            case 0:
                if (c == 'a') state = 1;
                else if (c == 'b') state = 0;
                else state = -1;
                break;
            case 1:
                if (c == 'a') state = 1;
                else if (c == 'b') state = 2;
                else state = -1;
                break;
            case 2:
                if (c == 'a') state = 1;
                else if (c == 'b') state = 3;
                else state = -1;
                break;
            case 3:
                state = -1;
                break;
            default:
                break;
        }
        if (state == -1) break;
    }

    if (state == 3) {
        printf("Accepted\n");
    } else {
        printf("Rejected\n");
    }

    return 0;
}