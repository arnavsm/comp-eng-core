%{
#include <stdio.h>
%}

%%
[+-]?[0-9]*\.[0-9]+([eE][+-]?[0-9]+)?[fFlL]?    { printf("Float: %s\n", yytext); }
[+-]?[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?[fFlL]?    { printf("Float: %s\n", yytext); }
[+-]?[0-9]+[eE][+-]?[0-9]+[fFlL]?               { printf("Float: %s\n", yytext); }
.|\n                                            { /* Ignore other characters */ }
%%

int main() {
    yylex();
    return 0;
}

int yywrap(){
    return 1;
}