%{
#include <stdio.h>
int count_zeros = 0;
int count_ones = 0;
%}

%%
"0"    { count_zeros++; }
"1"    { count_ones++; }
\n     { /* Check if counts match the desired pattern */
         if (count_zeros % 2 == 0 && count_ones % 2 == 1) {
             printf("Valid string\n");
             return 0;
         } else {
             printf("Invalid string\n");
             return 0;
         }
         count_zeros = 0; /* Reset counts for next line */
         count_ones = 0;
       }
.      { /* Ignore other characters */ }
%%

int main() {
    yylex();
    return 0;
}
