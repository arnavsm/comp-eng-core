%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);
extern int line_number;
%}

%union {
    char *sval;
}

%token <sval> ID CONST
%token PLUS TIMES
%token LPAREN RPAREN END

%left PLUS
%left TIMES

%%

program:
      expression END                 { printf("accept\n"); }
    | error END                      { yyclearin; yyerror("Syntax error"); }
    ;

expression:
      expression PLUS term            { }
    | term
    ;

term:
      term TIMES factor               { }
    | factor
    ;

factor:
      LPAREN expression RPAREN        { }
    | ID
    | CONST
    ;

%%

void yyerror(const char *s) {
    extern int line_number;
    fprintf(stderr, "Error: %s at position %d\n", s, line_number);
    exit(1);
}

int main(void) {
    yyparse();
    return 0;
}

