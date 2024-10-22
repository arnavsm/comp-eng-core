%{

#include <stdio.h>

#include <stdlib.h>

#include <string.h>


int yyle;

void yyerror(const char *s);


// Variable list to track unique variables

char variables[10]; // Limited to 10 variables for simplicity

int var_count = 0;

int satisfiable = 0; // To check if the expression is satisfiable


// Function prototypes

void add_variable(char var);

int find_variable(char var);

void generate_truth_table();

int eval_expression(int *values, const char *expression);

int implies(int p, int q);

int or_op(int p, int q);

int and_op(int p, int q);

int not_op(int p);


// Enum for operators

enum { NONE, IMPL, AND_OP, OR_OP, NOT_OP };

%}


%union {

char *s;

}


%token IMPLIES IFF AND OR NOT TRUE FALSE LPAREN RPAREN

%token <s> VAR


%%

expression:

term { generate_truth_table(); }

| term IMPLIES term { generate_truth_table(); }

| term IFF term { generate_truth_table(); }

;


term:

factor

| term AND factor { /* Handle AND operation */ }

| term OR factor { /* Handle OR operation */ }

;


factor:

VAR { add_variable(*$1); }

| TRUE { /* Handle true constant */ }

| FALSE { /* Handle false constant */ }

| NOT factor { /* Handle negation */ }

| LPAREN expression RPAREN { /* Handle parentheses */ }

;


%%


void yyerror(const char *s) {

fprintf(stderr, "Syntax Error: %s\n", s);

}


// Add variable to the list if it's not already present

void add_variable(char var) {

if (find_variable(var) == -1) {

variables[var_count++] = var;

}

}


// Find variable in the list, return its index or -1 if not found

int find_variable(char var) {

for (int i = 0; i < var_count; i++) {

if (variables[i] == var) return i;

}

return -1;

}


// Generate truth table and check satisfiability

void generate_truth_table() {

int rows = 1 << var_count; // 2^n combinations for n variables

int values[var_count];

satisfiable = 0; // Reset satisfiable status


printf("TRUTH TABLE:\n");

for (int i = 0; i < var_count; i++) {

printf("%c ", variables[i]);

}

printf("| Result\n");

printf("----------------\n");


// Loop over all combinations of truth values

for (int i = 0; i < rows; i++) {

for (int j = 0; j < var_count; j++) {

// Set values of variables based on the current row

values[j] = (i & (1 << (var_count - j - 1))) ? 1 : 0;

printf("%s ", values[j] ? "T" : "F");

}


// Evaluate the expression for the current truth assignment

int result = eval_expression(values, ""); // For now, pass empty string

printf("| %s\n", result ? "T" : "F");


if (result == 1) {

satisfiable = 1; // If at least one truth assignment is true, it's satisfiable

}

}


// Output if the expression is satisfiable or not

if (satisfiable) {

printf("The given w.f.f. is Satisfiable\n");

} else {

printf("The given w.f.f. is Unsatisfiable\n");

}

}


// Function to evaluate the expression for the current truth assignment

// For now, this is a simplified function that handles implications

int eval_expression(int *values, const char *expression) {

// Hardcoded for the example p->(q \/ ~r)

int p = values[find_variable('p')];

int q = values[find_variable('q')];

int r = values[find_variable('r')];


// Evaluate p -> (q \/ ~r)

return implies(p, or_op(q, not_op(r)));

}


// Helper functions for logic operators

int implies(int p, int q) {

return !p || q; // p -> q is equivalent to !p OR q

}


int or_op(int p, int q) {

return p || q;

}


int and_op(int p, int q) {

return p && q;

}


int not_op(int p) {

return !p;

}


int main(int argc, char *argv[]) {

printf("Enter a logical expression (w.f.f.): ");

yyparse();

return 0;

}
