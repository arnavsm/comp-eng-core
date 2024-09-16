/* just like Unix wc */
%{
   #include"anbn.tab.h" 
 // int yylval;    
%}
%%
"a"    { return A; } 
"b"    { return B; } 
\n     { return EOL; }
[^ab\n] {printf("LEX ERROR: %c is a wrong symbol\n",*yytext);}
%%
/*int main(int argc, char **argv)
{
yylex();
return 0;
}*/
