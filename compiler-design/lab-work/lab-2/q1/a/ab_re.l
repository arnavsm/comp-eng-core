%{
#include <stdio.h>

/* Define states:
   0: Initial state (and state after seeing 'a' or 'b' that's not part of 'abb')
   1: State after seeing first 'a' of potential 'abb'
   2: State after seeing 'ab' of potential 'abb'
   3: Final state, after seeing 'abb'
*/
int state = 0;

void transition(char input) {
    switch(state) {
        case 0:
            state = (input == 'a') ? 1 : 0;
            break;
        case 1:
            state = (input == 'b') ? 2 : ((input == 'a') ? 1 : 0);
            break;
        case 2:
            state = (input == 'b') ? 3 : ((input == 'a') ? 1 : 0);
            break;
        case 3:
            state = (input == 'a') ? 1 : 0;
            break;
    }
}

%}

%%

[ab]    { transition(yytext[0]); }
\n      { 
            if (state == 3)
                printf("Accepted\n");
            else
                printf("Rejected\n");
            state = 0;  // Reset state for next input
        }
.       { printf("Invalid input: %s\n", yytext); state = 0; }

%%

int main() {
    printf("Enter strings (one per line) of a's and b's:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}