#include <stdio.h>

int main() {
    char c;
    int state = 0;

    printf("Enter a string: ");
    while ((c = getchar()) != '\n' && c != EOF) {
        switch (state) {
            case 0:
                if (c == '0') state = 1;
                else if (c == '1') state = 2;
                else state = -1;
                break;
            case 1:
                if (c == '0') state = 0;
                else if (c == '1') state = 3;
                else state = -1;
                break;
            case 2:
                if (c == '0') state = 3;
                else if (c == '1') state = 0;
                else state = -1;
                break;
            case 3:
                if (c == '0') state = 2;
                else if (c == '1') state = 1;
                else state = -1;
                break;
            default:
                break;
        }
        if (state == -1) break;
    }

    if (state == 2) {
        printf("Accepted\n");
    } else {
        printf("Rejected\n");
    }

    return 0;
}