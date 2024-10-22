#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 26

typedef struct {
    int numStates;
    int numSymbols;
    bool transition[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
    bool isAccepting[MAX_STATES];
} NFA;

typedef struct {
    int numStates;
    int numSymbols;
    int transition[MAX_STATES][MAX_SYMBOLS];
    bool isAccepting[MAX_STATES];
} DFA;

NFA nfa;
DFA dfa;

void constructNFA(char* regex) {
    int len = strlen(regex);
    nfa.numStates = len + 1;
    nfa.numSymbols = 26; 

    memset(nfa.transition, 0, sizeof(nfa.transition));
    memset(nfa.isAccepting, 0, sizeof(nfa.isAccepting));

    for (int i = 0; i < len; i++) {
        if (regex[i] >= 'a' && regex[i] <= 'z') {
            nfa.transition[i][regex[i] - 'a'][i + 1] = true;
        }
    }
    nfa.isAccepting[len] = true;
}

void epsilonClosure(bool* states, bool* result) {
    memcpy(result, states, sizeof(bool) * MAX_STATES);
}

void NFAtoDFA() {
    bool currentStates[MAX_STATES] = {0};
    bool nextStates[MAX_STATES] = {0};
    int dfaStates[MAX_STATES][MAX_STATES] = {0};
    int numDFAStates = 0;

    currentStates[0] = true;
    epsilonClosure(currentStates, nextStates);
    memcpy(dfaStates[numDFAStates], nextStates, sizeof(bool) * MAX_STATES);
    numDFAStates++;

    for (int i = 0; i < numDFAStates; i++) {
        for (int j = 0; j < nfa.numSymbols; j++) {
            memset(nextStates, 0, sizeof(nextStates));
            for (int k = 0; k < nfa.numStates; k++) {
                if (dfaStates[i][k]) {
                    for (int l = 0; l < nfa.numStates; l++) {
                        if (nfa.transition[k][j][l]) {
                            nextStates[l] = true;
                        }
                    }
                }
            }
            epsilonClosure(nextStates, nextStates);

            bool isNewState = true;
            for (int k = 0; k < numDFAStates; k++) {
                if (memcmp(dfaStates[k], nextStates, sizeof(bool) * MAX_STATES) == 0) {
                    dfa.transition[i][j] = k;
                    isNewState = false;
                    break;
                }
            }
            if (isNewState) {
                memcpy(dfaStates[numDFAStates], nextStates, sizeof(bool) * MAX_STATES);
                dfa.transition[i][j] = numDFAStates;
                numDFAStates++;
            }
        }
    }

    dfa.numStates = numDFAStates;
    dfa.numSymbols = nfa.numSymbols;

    for (int i = 0; i < dfa.numStates; i++) {
        for (int j = 0; j < nfa.numStates; j++) {
            if (dfaStates[i][j] && nfa.isAccepting[j]) {
                dfa.isAccepting[i] = true;
                break;
            }
        }
    }
}

bool areDistinguishable(int s1, int s2) {
    if (dfa.isAccepting[s1] != dfa.isAccepting[s2]) return true;
    for (int i = 0; i < dfa.numSymbols; i++) {
        if (dfa.transition[s1][i] != dfa.transition[s2][i]) return true;
    }
    return false;
}

void minimizeDFA() {
    bool distinguishable[MAX_STATES][MAX_STATES] = {0};
    int newState[MAX_STATES] = {0};

    for (int i = 0; i < dfa.numStates; i++) {
        for (int j = i + 1; j < dfa.numStates; j++) {
            distinguishable[i][j] = areDistinguishable(i, j);
            distinguishable[j][i] = distinguishable[i][j];
        }
    }

    bool changed;
    do {
        changed = false;
        for (int i = 0; i < dfa.numStates; i++) {
            for (int j = i + 1; j < dfa.numStates; j++) {
                if (!distinguishable[i][j]) {
                    for (int k = 0; k < dfa.numSymbols; k++) {
                        int next_i = dfa.transition[i][k];
                        int next_j = dfa.transition[j][k];
                        if (distinguishable[next_i][next_j]) {
                            distinguishable[i][j] = true;
                            distinguishable[j][i] = true;
                            changed = true;
                            break;
                        }
                    }
                }
            }
        }
    } while (changed);

    int numNewStates = 0;
    for (int i = 0; i < dfa.numStates; i++) {
        if (newState[i] == 0) {
            numNewStates++;
            newState[i] = numNewStates;
            for (int j = i + 1; j < dfa.numStates; j++) {
                if (!distinguishable[i][j]) {
                    newState[j] = newState[i];
                }
            }
        }
    }

    DFA minDFA;
    minDFA.numStates = numNewStates;
    minDFA.numSymbols = dfa.numSymbols;

    for (int i = 0; i < dfa.numStates; i++) {
        if (newState[i] == i + 1) {
            minDFA.isAccepting[newState[i] - 1] = dfa.isAccepting[i];
            for (int j = 0; j < dfa.numSymbols; j++) {
                minDFA.transition[newState[i] - 1][j] = newState[dfa.transition[i][j]] - 1;
            }
        }
    }

    dfa = minDFA;
}

void printDFA() {
    printf("Minimized DFA:\n");
    printf("Number of states: %d\n", dfa.numStates);
    printf("Transition table:\n");
    printf("   ");
    for (int i = 0; i < dfa.numSymbols; i++) {
        printf("%c  ", 'a' + i);
    }
    printf("\n");
    for (int i = 0; i < dfa.numStates; i++) {
        printf("%d: ", i);
        for (int j = 0; j < dfa.numSymbols; j++) {
            printf("%d  ", dfa.transition[i][j]);
        }
        if (dfa.isAccepting[i]) printf(" (accepting)");
        printf("\n");
    }
}

int main() {
    char regex[100];
    printf("Enter the regular expression: ");
    scanf("%s", regex);

    constructNFA(regex);
    NFAtoDFA();
    minimizeDFA();
    printDFA();

    return 0;
}