%{
#include "expr.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line_number = 1;
%}

%%
[ \t]+                  ;  // Ignore whitespace
\n                      { line_number++; }
\+                      { return PLUS; }
\*                      { return TIMES; }
\(                      { return LPAREN; }
\)                      { return RPAREN; }
[a-zA-Z][a-zA-Z0-9]*    { yylval.sval = strdup(yytext); return ID; }
[0-9]+                  { yylval.sval = strdup(yytext); return CONST; }
\$                      { return END; }
.                       { printf("Error: Unexpected character %s at position %d\n", yytext, yyleng); exit(1); }
%%

int yywrap(void) {
    return 1;
}

