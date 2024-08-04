/* just like Unix wc */
%{

%}
%%
00(0|1)*11 { printf("\nMatch"); }
%%
int main(int argc, char **argv)
{
yylex();
return 0;
}
