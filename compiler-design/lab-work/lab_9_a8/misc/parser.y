%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int temp_count = 0; // Counter for temporary variables

void generate_code(const char* op, const char* arg1, const char* arg2, const char* result) {
    printf("%s = %s %s %s\n", result, arg1, op, arg2);
}

// Declare yyerror function
void yyerror(const char *s);
%}

%union {
    char* str;
    int num;
}

%token <str> IDENTIFIER
%token <num> NUMBER
%token PLUS MINUS MULTIPLY DIVIDE MODULO ASSIGN
%token LPAREN RPAREN

%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%right ASSIGN
%nonassoc UNARY

%type <str> expression

%% 

program:
    expression { /* Nothing needed here for now */ }
    ;

expression:
    expression PLUS expression {
        char *result = malloc(10);
        sprintf(result, "t%d", ++temp_count);
        generate_code("+", $1, $3, result);
        $$ = result; // Set $$ to result
    }
    | expression MINUS expression {
        char *result = malloc(10);
        sprintf(result, "t%d", ++temp_count);
        generate_code("-", $1, $3, result);
        $$ = result; // Set $$ to result
    }
    | expression MULTIPLY expression {
        char *result = malloc(10);
        sprintf(result, "t%d", ++temp_count);
        generate_code("*", $1, $3, result);
        $$ = result; // Set $$ to result
    }
    | expression DIVIDE expression {
        char *result = malloc(10);
        sprintf(result, "t%d", ++temp_count);
        generate_code("/", $1, $3, result);
        $$ = result; // Set $$ to result
    }
    | MINUS expression %prec UNARY {
        char *result = malloc(10);
        sprintf(result, "t%d", ++temp_count);
        generate_code("-", "0", $2, result);
        $$ = result; // Set $$ to result
    }
    | LPAREN expression RPAREN {
        $$ = $2; // Just return the inner expression
    }
    | IDENTIFIER {
        $$ = strdup($1); // Return a copy of the identifier
    }
    | NUMBER {
        char *result = malloc(10);
        sprintf(result, "%d", $1); // Convert number to string
        $$ = result; // Return the number
    }
    ;

%%

// Entry point
int main() {
    printf("Three-address code generation:\n");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
