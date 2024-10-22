#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
#define MAX_RULES 10

typedef struct {
    char nonTerminal;
    char productions[MAX_RULES][MAX];
    int productionCount;
} Grammar;

void addProduction(Grammar *grammar, const char *prod) {
    strcpy(grammar->productions[grammar->productionCount++], prod);
}

void printProductions(const Grammar *grammar) {
    printf("%c -> ", grammar->nonTerminal);
    for (int i = 0; i < grammar->productionCount; i++) {
        printf("%s", grammar->productions[i]);
        if (i < grammar->productionCount - 1) {
            printf(" | ");
        }
    }
    printf("\n");
}

int isDirectLeftRecursive(const Grammar *grammar) {
    for (int i = 0; i < grammar->productionCount; i++) {
        if (grammar->productions[i][0] == grammar->nonTerminal) {
            return 1; // Direct left recursion detected
        }
    }
    return 0;
}

void eliminateDirectLeftRecursion(Grammar *grammar) {
    Grammar newGrammar;
    newGrammar.nonTerminal = grammar->nonTerminal + 1;
    newGrammar.productionCount = 0;

    char newNonTerminal = newGrammar.nonTerminal;
    
    // Handle non-recursive productions
    Grammar recursivePart;
    recursivePart.nonTerminal = grammar->nonTerminal;
    recursivePart.productionCount = 0;

    // Collect recursive and non-recursive productions
    for (int i = 0; i < grammar->productionCount; i++) {
        if (grammar->productions[i][0] == grammar->nonTerminal) {
            // Left-recursive production
            char newProduction[MAX];
            snprintf(newProduction, MAX, "%s%c", grammar->productions[i] + 1, newNonTerminal);
            addProduction(&newGrammar, newProduction);
        } else {
            // Non-left-recursive production
            char newProduction[MAX];
            snprintf(newProduction, MAX, "%s%c", grammar->productions[i], newNonTerminal);
            addProduction(&recursivePart, newProduction);
        }
    }

    // Add epsilon (ε) production to the new non-terminal
    addProduction(&newGrammar, "ε");

    // Print the transformed grammar
    printf("Grammar after eliminating left recursion:\n");

    // Print the original non-terminal productions
    printf("%c -> ", grammar->nonTerminal);
    for (int i = 0; i < recursivePart.productionCount; i++) {
        printf("%s", recursivePart.productions[i]);
        if (i < recursivePart.productionCount - 1) {
            printf(" | ");
        }
    }
    printf("| %c\n", newNonTerminal);

    // Print the new non-terminal productions
    printProductions(&newGrammar);
}

int main() {
    Grammar grammar;
    grammar.nonTerminal = 'A'; // Example non-terminal
    grammar.productionCount = 0;

    printf("Enter the number of productions for grammar A: ");
    int numProductions;
    scanf("%d", &numProductions);

    printf("Enter the productions (e.g., Aab, Ab):\n");
    for (int i = 0; i < numProductions; i++) {
        char prod[MAX];
        scanf("%s", prod);
        addProduction(&grammar, prod);
    }

    if (isDirectLeftRecursive(&grammar)) {
        printf("The grammar is left-recursive.\n");
        eliminateDirectLeftRecursion(&grammar);
    } else {
        printf("The grammar is not left-recursive.\n");
    }

    return 0;
}

