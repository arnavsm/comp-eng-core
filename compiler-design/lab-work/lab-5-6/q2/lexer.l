%{
#include <stdio.h>
#include <string.h>
#include "parser.tab.h"

extern void yyerror(const char *s);
int input_complete = 0;
%}

%option noyywrap

%%

"int"|"char"|"float"|"unsigned int"|"long int"|"short int"|"double"|"long double" { yylval.strval = strdup(yytext); return DATATYPE; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.strval = strdup(yytext); return IDENTIFIER; }
";" { input_complete = 1; return SEMICOLON; }
"," { return COMMA; }
[ \t]   /* Ignore whitespace */
\n      { 
    if (!input_complete) {
        yyerror("Error: Incomplete declaration");
        input_complete = 1;
        return SEMICOLON;
    }
    input_complete = 0;
    return 0;  // Signals end of input to Bison
}
<<EOF>> { 
    if (!input_complete) {
        yyerror("Error: Incomplete declaration");
        input_complete = 1;
        return SEMICOLON;
    }
    yyterminate();
}
. { yylval.strval = strdup(yytext); return UNKNOWN; }

%%
