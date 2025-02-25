%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
%}

%%
[0-9]+      { yylval.num = atoi(yytext); return NUMBER; }
[A-Z]      { yylval.str = strdup(yytext); return IDENTIFIER; }
"+"        { return PLUS; }
"-"        { return MINUS; }
"*"        { return MULTIPLY; }
"/"        { return DIVIDE; }
"%"        { return MODULO; }
"="        { return ASSIGN; }
"("        { return LPAREN; }
")"        { return RPAREN; }
[ \t\n]+   { /* ignore whitespace */ }
.          { printf("Invalid character: %s\n", yytext); }

%%

// Entry point
int yywrap() {
    return 1;
}
