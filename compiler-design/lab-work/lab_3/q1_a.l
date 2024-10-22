%{
#include <stdio.h>
int count_a = 0, count_b = 0;
%}

%%
a   { count_a++; }
b   { count_b++; }
\n  { 
    if (abs(count_a - count_b) % 3 == 0)
        printf("Accepted\n");
    else
        printf("Rejected\n");
    count_a = count_b = 0;
    }
.   { /* Ignore other characters */ }
%%

int main() {
    yylex();
    return 0;
}
int yywrap(){
    return 1;
}