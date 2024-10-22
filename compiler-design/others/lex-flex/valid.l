%{
#include <stdio.h>
%}

%%

([A-Za-z_][A-Za-z0-9_@]*) { printf("Valid identifier: %s\n", yytext); }
.                          { printf("Invalid identifier: %s\n", yytext); }

%%

int main() {
    yylex();
    return 0;
}