%option noyywrap

%{
#include <stdio.h>
int state = 0;
%}

%%
[CHNIOISLT] {
    if (state == 0 || state == 1) {
        state = 3;
    }
    else {
        state = 4;
    }
}
[0-9]   {
    if (state == 3) {
        state = 2;
    }
    else {
        state = 4;
    }
}
","   {
    if (state == 2 || state == 3) {
        state = 1;
    }
    else {
        state = 4;
    }
}
\n  { 
    if (state == 2 || state == 3) {
        printf("Valid Formula\n");
    }
    else {
        printf("Invalid Formula\n");
    }
    state = 0;
}
.   { state = 4; }
%%

int main() {
    yylex();
    return 0;
}