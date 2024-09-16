#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define MAX_TERMINALS 50
#define MAX_NON_TERMINALS 10

typedef struct {
    char nonTerminal;
    char productions[MAX][MAX];
    int productionCount;
} Grammar;

Grammar grammar[MAX_NON_TERMINALS];
int grammarCount = 0;

int firstSets[MAX_NON_TERMINALS][MAX_TERMINALS];
int followSets[MAX_NON_TERMINALS][MAX_TERMINALS];
int firstCount[MAX_NON_TERMINALS];
int followCount[MAX_NON_TERMINALS];
int terminalsCount = 0;
char terminals[MAX_TERMINALS];
char nonTerminals[MAX_NON_TERMINALS];
int terminalIndex(char terminal) {
    for (int i = 0; i < terminalsCount; i++) {
        if (terminals[i] == terminal) return i;
    }
    return -1;
}
int nonTerminalIndex(char nonTerminal) {
    for (int i = 0; i < grammarCount; i++) {
        if (grammar[i].nonTerminal == nonTerminal) return i;
    }
    return -1;
}

void addTerminal(char terminal) {
    if (terminalIndex(terminal) == -1) {
        terminals[terminalsCount++] = terminal;
    }
}

void addNonTerminal(char nonTerminal) {
    if (nonTerminalIndex(nonTerminal) == -1) {
        nonTerminals[grammarCount++] = nonTerminal;
    }
}

void computeFirst() {
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < grammarCount; i++) {
            Grammar *g = &grammar[i];
            for (int j = 0; j < g->productionCount; j++) {
                char *prod = g->productions[j];
                if (isalpha(prod[0])) {
                    int terminalIdx = terminalIndex(prod[0]);
                    if (terminalIdx != -1) {
                        if (firstSets[i][terminalIdx] == 0) {
                            firstSets[i][terminalIdx] = 1;
                            firstCount[i]++;
                            changed = 1;
                        }
                    } else {
                        int nonTerminalIdx = nonTerminalIndex(prod[0]);
                        for (int k = 0; k < firstCount[nonTerminalIdx]; k++) {
                            if (firstSets[nonTerminalIdx][k] && !firstSets[i][k]) {
                                firstSets[i][k] = 1;
                                firstCount[i]++;
                                changed = 1;
                            }
                        }
                        if (strchr(prod, '\0')) {
                            if (!firstSets[i][terminalsCount]) {
                                firstSets[i][terminalsCount] = 1;
                                firstCount[i]++;
                                changed = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void computeFollow() {
    int changed = 1;
    followSets[0][terminalIndex('$')] = 1;
    followCount[0] = 1;
    
    while (changed) {
        changed = 0;
        for (int i = 0; i < grammarCount; i++) {
            Grammar *g = &grammar[i];
            for (int j = 0; j < g->productionCount; j++) {
                char *prod = g->productions[j];
                for (int k = 0; prod[k]; k++) {
                    if (isalpha(prod[k]) && !isupper(prod[k])) {
                        int nonTerminalIdx = nonTerminalIndex(prod[k]);
                        for (int l = 0; l < terminalsCount; l++) {
                            if (followSets[nonTerminalIdx][l] == 0 && followSets[i][l]) {
                                followSets[nonTerminalIdx][l] = 1;
                                followCount[nonTerminalIdx]++;
                                changed = 1;
                            }
                        }
                        if (strchr(prod, '\0')) {
                            int ntIdx = nonTerminalIndex(prod[k]);
                            for (int l = 0; l < followCount[ntIdx]; l++) {
                                if (followSets[i][l] && !followSets[ntIdx][l]) {
                                    followSets[ntIdx][l] = 1;
                                    followCount[ntIdx]++;
                                    changed = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void printSets() {
    printf("FIRST sets:\n");
    for (int i = 0; i < grammarCount; i++) {
        printf("FIRST(%c) = { ", grammar[i].nonTerminal);
        for (int j = 0; j < terminalsCount; j++) {
            if (firstSets[i][j]) printf("%c ", terminals[j]);
        }
        printf("}\n");
    }

    printf("FOLLOW sets:\n");
    for (int i = 0; i < grammarCount; i++) {
        printf("FOLLOW(%c) = { ", grammar[i].nonTerminal);
        for (int j = 0; j < terminalsCount; j++) {
            if (followSets[i][j]) printf("%c ", terminals[j]);
        }
        printf("}\n");
    }
}

int isLL1() {
    for (int i = 0; i < grammarCount; i++) {
        for (int j = 0; j < grammar[i].productionCount; j++) {
            char *prod = grammar[i].productions[j];
            for (int k = j + 1; k < grammar[i].productionCount; k++) {
                char *prod2 = grammar[i].productions[k];
                if (prod[0] == prod2[0]) {
                    return 0; // Conflict in FIRST sets
                }
            }
        }
    }

    for (int i = 0; i < grammarCount; i++) {
        for (int j = 0; j < grammar[i].productionCount; j++) {
            char *prod = grammar[i].productions[j];
            if (strchr(prod, '\0')) {
                int ntIdx = nonTerminalIndex(grammar[i].nonTerminal);
                for (int k = 0; k < terminalsCount; k++) {
                    if (firstSets[ntIdx][k] && followSets[i][k]) {
                        return 0; // Conflict in FOLLOW sets
                    }
                }
            }
        }
    }

    return 1;
}

int main() {
    printf("Enter the number of non-terminals: ");
    scanf("%d", &grammarCount);
    getchar(); // Consume newline

    for (int i = 0; i < grammarCount; i++) {
        Grammar *g = &grammar[i];
        printf("Enter the non-terminal (single character): ");
        g->nonTerminal = getchar();
        getchar(); // Consume newline

        addNonTerminal(g->nonTerminal);
        g->productionCount = 0;

        printf("Enter the number of productions for %c: ", g->nonTerminal);
        int numProductions;
        scanf("%d", &numProductions);
        getchar(); // Consume newline

        for (int j = 0; j < numProductions; j++) {
            printf("Enter production %d: ", j + 1);
            fgets(g->productions[j], MAX, stdin);
            g->productions[j][strcspn(g->productions[j], "\n")] = '\0'; // Remove newline
            for (int k = 0; g->productions[j][k]; k++) {
                if (!isupper(g->productions[j][k])) {
                    addTerminal(g->productions[j][k]);
                }
            }
        }
    }

    // Initialize sets
    for (int i = 0; i < grammarCount; i++) {
        firstCount[i] = 0;
        followCount[i] = 0;
        for (int j = 0; j < terminalsCount; j++) {
            firstSets[i][j] = 0;
            followSets[i][j] = 0;
        }
    }

    computeFirst();
    computeFollow();
    printSets();

    if (isLL1()) {
        printf("The grammar is LL(1).\n");
    } else {
        printf("The grammar is not LL(1).\n");
    }

    return 0;
}
