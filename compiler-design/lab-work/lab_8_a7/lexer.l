%{

#include "parser.tab.h"

#include <stdio.h>

#include <stdlib.h>

%}


DIGIT [0-9]

LETTER [A-Za-z]


%%

"->" { return IMPLIES; }

"<->" { return IFF; }

"/\\" { return AND; }

"\\/" { return OR; }

"~" { return NOT; }

"T"|"t" { return TRUE; }

"F"|"f" { return FALSE; }

"(" { return LPAREN; }

")" { return RPAREN; }

{LETTER} { yylval.s = strdup(yytext); return VAR; }

[ \t\n] ; // Ignore whitespace

. { printf("Lexical Error at position %d: invalid token '%s'\n", yylineno, yytext); return 0; }


%%

int yywrap() { return 1; }
