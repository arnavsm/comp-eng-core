/* just like Unix wc */
%{

%}
%%
[a-zA-Z]([a-zA-Z]|[0-9])+ { printf("\nValid identifier"); }
%%
int main(int argc, char **argv)
{
yylex();
return 0;
}
