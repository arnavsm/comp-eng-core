%{
#include <stdio.h>
%}

%%

([^\@1]*)         { /* Ignore any characters except '@' and '1' */ }
(@[^\@1]*)        { yymore(); if (++num_ats % 2 == 1) odd_at = 1; else odd_at = 0; }
(1[^\@1]*)        { yymore(); if (++num_ones % 2 == 0) even_1 = 1; else even_1 = 0; }
\n                { if (odd_at && even_1) printf("Valid\n"); else printf("Invalid\n"); }
.                 { /* Ignore any other characters */ }

%%

int main() {
    num_ats = num_ones = 0;
    odd_at = even_1 = 0;
    yylex();
    return 0;
}