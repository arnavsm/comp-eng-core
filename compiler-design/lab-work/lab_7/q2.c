#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
#define MAX_PRODUCTIONS 10

typedef struct {
    char nonTerminal;
    char productions[MAX_PRODUCTIONS][MAX];
    int productionCount;
} Grammar;

void addProduction(Grammar *grammar, const char *prod) {
    if (grammar->productionCount < MAX_PRODUCTIONS) {
        strcpy(grammar->productions[grammar->productionCount++], prod);
    }
}

void factorLeft(Grammar *grammar) {
    printf("Left factoring for non-terminal %c:\n", grammar->nonTerminal);

    // Find the longest common prefix
    int maxPrefixLength = 0;
    char prefix[MAX] = "";

    for (int i = 0; i < grammar->productionCount; i++) {
        for (int j = i + 1; j < grammar->productionCount; j++) {
            int k = 0;
            // Find the common prefix length
            while (grammar->productions[i][k] && grammar->productions[j][k] &&
                   grammar->productions[i][k] == grammar->productions[j][k]) {
                k++;
            }

            // Update the maximum prefix length and prefix itself
            if (k > maxPrefixLength) {
                maxPrefixLength = k;
                strncpy(prefix, grammar->productions[i], k);
                prefix[k] = '\0';
            }
        }
    }

    // If we have a common prefix
    if (maxPrefixLength > 0) {
        char newNonTerminal = grammar->nonTerminal + 1;
        printf("Common prefix found: %s\n", prefix);
        printf("Creating new non-terminal: %c\n", newNonTerminal);

        // Print the factored production
        printf("%c -> %s%c'\n", grammar->nonTerminal, prefix, newNonTerminal);

        // Print the new non-terminal productions
        printf("%c' -> ", newNonTerminal);
        int hasProduction = 0;
        for (int i = 0; i < grammar->productionCount; i++) {
            if (strncmp(grammar->productions[i], prefix, maxPrefixLength) == 0) {
                if (grammar->productions[i][maxPrefixLength] != '\0') {
                    printf("%s ", grammar->productions[i] + maxPrefixLength);
                } else {
                    printf("ε "); // ε denotes an empty string
                }
                hasProduction = 1;
            }
        }
        if (!hasProduction) {
            printf("ε ");
        }
        printf("\n");
    } else {
        printf("No common prefix found.\n");
    }
}

int main() {
    Grammar grammar;
    grammar.nonTerminal = 'A'; // Example non-terminal
    grammar.productionCount = 0;

    printf("Enter the number of productions for non-terminal %c: ", grammar.nonTerminal);
    int numProductions;
    scanf("%d", &numProductions);

    printf("Enter the productions (e.g., Aa, Ab):\n");
    for (int i = 0; i < numProductions; i++) {
        char prod[MAX];
        scanf("%s", prod);
        addProduction(&grammar, prod);
    }

    factorLeft(&grammar);

    return 0;
}


