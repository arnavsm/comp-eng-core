/* just like Unix wc */
%{

%}
%%
(a|b)*abb { printf("\nCorrect"); }
%%
int main(int argc, char **argv)
{
yylex();
return 0;
}