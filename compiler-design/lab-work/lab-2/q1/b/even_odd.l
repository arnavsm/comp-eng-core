%{
#include <stdio.h>

/* Define states:
   Q0: Even 0's, Even 1's (Initial state)
   Q1: Even 0's, Odd 1's  (Final state)
   Q2: Odd 0's, Even 1's
   Q3: Odd 0's, Odd 1's
*/
int state = 0;

void transition(char input) {
    switch(state) {
        case 0:
            state = (input == '0') ? 2 : 1;
            break;
        case 1:
            state = (input == '0') ? 3 : 0;
            break;
        case 2:
            state = (input == '0') ? 0 : 3;
            break;
        case 3:
            state = (input == '0') ? 1 : 2;
            break;
    }
}

%}

%%

[01]    { transition(yytext[0]); }
\n      { 
            if (state == 1)
                printf("Accepted\n");
            else
                printf("Rejected\n");
            state = 0;  // Reset state for next input
        }
.       { printf("Invalid input: %s\n", yytext); }

%%

int main() {
    printf("Enter strings (one per line) with 0's and 1's:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}