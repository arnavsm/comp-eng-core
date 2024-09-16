%{
#include <stdio.h>
%}

%%
-?[1-9][0-9]*[UL]?           { printf("Decimal: %s\n", yytext); }
0[0-7]*[UL]?                 { printf("Octal: %s\n", yytext); }
0[xX][0-9a-fA-F]+[UL]?       { printf("Hexadecimal: %s\n", yytext); }
.|\n                         { /* Ignore other characters */ }
%%

int main() {
    yylex();
    return 0;
}