/* just like Unix wc */
%{

%}
%%
int { printf("\n key word"); }
[0-9][0-9]* { printf("\n constant"); }
[a-zA-Z]([a-zA-Z]|[0-9])* { printf("\n identifier"); }
%%
int main(int argc, char **argv)
{
yylex();
return 0;
}
