/* simplest version of calculator */
%{
#include <stdio.h>
#include<stdlib.h>
int yylex(void);
void yyerror(char*);
unsigned int a_count,b_count,wrong_sym_count=0;
%}
/* declare tokens */
%token A
%token B
%token EOL
%token WRONG_SYMBOL
%%
stmt: S EOL  { if(wrong_sym_count>0)
                  printf("valid string\n"); 
               exit(0); }      
        ; 
S:      A S B  { printf("S->aSb\n"); a_count++;b_count++; }
        | { printf("S->epsilon\n");a_count=0;b_count=0; }
        | WRONG_SYMBOL {wrong_sym_count++;}
        ;
%%
int main(int argc, char **argv)
{
  yyparse();
 return 0; 
}
void yyerror(char *s)
{
// fprintf(stderr, "error: %s\n", s);
printf("SYNTAX ERROR: ");
   if(wrong_sym_count>0)
      if(a_count>b_count)
                 printf("#as > #bs\n");
               else
                 printf("#as < #bs\n"); 
}
