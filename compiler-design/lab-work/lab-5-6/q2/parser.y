%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE* yyin;
void yyerror(const char *s);

#define MAX_VARS 100

char *variables[MAX_VARS];
int var_count = 0;
char *current_datatype = NULL;
int error_occurred = 0;

void add_variable(char *var);
int check_variable(char *var);
void free_variables();
%}

%union {
    char *strval;
}

%token <strval> DATATYPE IDENTIFIER UNKNOWN
%token SEMICOLON COMMA

%type <strval> declaration

%%

input: declaration SEMICOLON {
         if (!error_occurred) {
             printf("accept\n");
         }
         free_variables();
         if (current_datatype) free(current_datatype);
         current_datatype = NULL;
         YYACCEPT;
       }
     | error SEMICOLON {
         free_variables();
         if (current_datatype) free(current_datatype);
         current_datatype = NULL;
         YYABORT;
       }
     ;

declaration: DATATYPE { current_datatype = strdup($1); } var_list
           | UNKNOWN { yyerror("Error: unknown datatype"); }
           | IDENTIFIER { yyerror("Error: datatype of identifier missing"); }
           ;

var_list: IDENTIFIER { add_variable($1); }
        | var_list COMMA IDENTIFIER { add_variable($3); }
        ;

%%

void add_variable(char *var) {
    if (check_variable(var)) {
        char error_msg[100];
        snprintf(error_msg, sizeof(error_msg), "Error: %s is already a/redeclaration of %s", var, var);
        yyerror(error_msg);
    } else if (var_count < MAX_VARS) {
        variables[var_count++] = strdup(var);
    } else {
        yyerror("Error: too many variables");
    }
}

int check_variable(char *var) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i], var) == 0) {
            return 1;
        }
    }
    return 0;
}

void free_variables() {
    for (int i = 0; i < var_count; i++) {
        free(variables[i]);
    }
    var_count = 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
    error_occurred = 1;
}

int main() {
    char input[1000];
    while (1) {
        printf("Input: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;  // Exit on EOF
        }
        yyin = fmemopen(input, strlen(input), "r");
        if (yyin == NULL) {
            perror("fmemopen");
            exit(1);
        }
        error_occurred = 0;  // Reset error flag before each parse
        yyparse();
        fclose(yyin);
        yylex(); // Call yylex() once to reset its internal state
        printf("\n");
    }
    return 0;
}
