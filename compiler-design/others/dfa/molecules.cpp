/*

Design a DFA in C code that recognizes chemical formulas composed of the following 8 elements: H, C, N, O, Si, S, Cl, and Sn. 
The elements are separated by commas, and they can appear in any order and combination. Each element can be followed by an 
optional digit representing the number of atoms. The input symbols are: C H I L O N S , d (where d represents any digit). 
The DFA should accept formulas that are syntactically correct, even if they do not represent actual compounds.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 9

// Define states
enum State {
    START,
    H, C, N, O, S, SI, CL, SN,
    H_NUM, C_NUM, N_NUM, O_NUM, S_NUM, SI_NUM, CL_NUM, SN_NUM,
    INVALID
};

// Define input symbols
enum Symbol {
    SYM_H, SYM_C, SYM_N, SYM_O, SYM_S, SYM_I, SYM_L, SYM_COMMA, SYM_DIGIT
};

// DFA transition function
int transition[MAX_STATES][MAX_SYMBOLS];

// Initialize the DFA
void init_dfa() {
    memset(transition, INVALID, sizeof(transition));

    // Define transitions for each state
    for (int i = START; i <= SN; i++) {
        transition[i][SYM_H] = H;
        transition[i][SYM_C] = C;
        transition[i][SYM_N] = N;
        transition[i][SYM_O] = O;
        transition[i][SYM_S] = S;
        transition[i][SYM_COMMA] = START;
    }

    // Special transitions for SI, CL, SN
    for (int i = START; i <= SN; i++) {
        transition[i][SYM_I] = SI;
        transition[i][SYM_L] = CL;
    }
    transition[S][SYM_N] = SN;

    // Transitions for digits
    transition[H][SYM_DIGIT] = H_NUM;
    transition[C][SYM_DIGIT] = C_NUM;
    transition[N][SYM_DIGIT] = N_NUM;
    transition[O][SYM_DIGIT] = O_NUM;
    transition[S][SYM_DIGIT] = S_NUM;
    transition[SI][SYM_DIGIT] = SI_NUM;
    transition[CL][SYM_DIGIT] = CL_NUM;
    transition[SN][SYM_DIGIT] = SN_NUM;

    // Self-loops for number states
    for (int i = H_NUM; i <= SN_NUM; i++) {
        transition[i][SYM_DIGIT] = i;
        transition[i][SYM_COMMA] = START;
    }
}

// Convert input character to symbol
int char_to_symbol(char c) {
    switch (c) {
        case 'H': return SYM_H;
        case 'C': return SYM_C;
        case 'N': return SYM_N;
        case 'O': return SYM_O;
        case 'S': return SYM_S;
        case 'I': return SYM_I;
        case 'L': return SYM_L;
        case ',': return SYM_COMMA;
        default: return isdigit(c) ? SYM_DIGIT : -1;
    }
}

// Check if the formula is valid
int is_valid_formula(const char* formula) {
    int state = START;
    int len = strlen(formula);

    for (int i = 0; i < len; i++) {
        int symbol = char_to_symbol(formula[i]);
        if (symbol == -1) return 0;  // Invalid character

        state = transition[state][symbol];
        if (state == INVALID) return 0;
    }

    return state != INVALID && state != SI && state != CL;
}

int main() {
    init_dfa();

    char formula[100];
    printf("Enter a chemical formula: ");
    scanf("%s", formula);

    if (is_valid_formula(formula)) {
        printf("Valid formula\n");
    } else {
        printf("Invalid formula\n");
    }

    return 0;
}

/*

This C program implements a DFA that recognizes chemical formulas based on the given constraints. Here's a breakdown of the code:

1. We define states for each element and their corresponding number states.
2. We define input symbols for each element, comma, and digit.
3. The `transition` array represents the DFA's transition function.
4. The `init_dfa()` function initializes the transition function.
5. The `char_to_symbol()` function converts input characters to symbols.
6. The `is_valid_formula()` function checks if a given formula is valid by simulating the DFA.
7. The `main()` function initializes the DFA and prompts the user for input.

This DFA recognizes formulas with the following properties:
- It accepts the elements H, C, N, O, SI, S, CL, and Sn in any order.
- Elements can be separated by commas.
- Elements can have optional numbers following them.
- The formula doesn't need to represent compounds that actually exist.

To use this program, compile and run it. Then enter a chemical formula when prompted. 
The program will output whether the formula is valid or invalid according to the defined rules.

*/