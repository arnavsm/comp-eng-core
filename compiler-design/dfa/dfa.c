#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STATES 1000
#define MAX_SYMBOLS 26
#define MAX_STACK 1000

typedef struct {
    int transitions[MAX_SYMBOLS];
    bool is_final;
} State;

typedef struct {
    State states[MAX_STATES];
    int num_states;
    int start_state;
    char alphabet[MAX_SYMBOLS];
    int alphabet_size;
} DFA;

char stack[MAX_STACK];
int top = -1;

void push(char c) {
    stack[++top] = c;
}

char pop() {
    return stack[top--];
}

int precedence(char c) {
    switch(c) {
        case '*': return 3;
        case '.': return 2;
        case '|': return 1;
        default: return 0;
    }
}

void re_to_postfix(char* re, char* postfix) {
    char c, stack[MAX_STACK];
    int i, j, top = -1;

    for (i = 0, j = 0; re[i] != '\0'; i++) {
        c = re[i];
        switch(c) {
            case '(':
                push(c);
                break;
            case ')':
                while (top > -1 && stack[top] != '(')
                    postfix[j++] = pop();
                if (top > -1 && stack[top] == '(')
                    pop();
                break;
            case '*':
            case '.':
            case '|':
                while (top > -1 && precedence(stack[top]) >= precedence(c))
                    postfix[j++] = pop();
                push(c);
                break;
            default:
                postfix[j++] = c;
        }
    }

    while (top > -1)
        postfix[j++] = pop();
    postfix[j] = '\0';
}

DFA* build_nfa(char* postfix) {
    DFA* nfa = malloc(sizeof(DFA));
    nfa->num_states = 0;
    nfa->start_state = 0;
    nfa->alphabet_size = 0;

    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];
        if (c >= 'a' && c <= 'z') {
            int symbol = c - 'a';
            if (nfa->alphabet[symbol] == 0) {
                nfa->alphabet[symbol] = 1;
                nfa->alphabet_size++;
            }
            State start = {.is_final = false};
            State end = {.is_final = true};
            start.transitions[symbol] = nfa->num_states + 1;
            nfa->states[nfa->num_states++] = start;
            nfa->states[nfa->num_states++] = end;
        } else if (c == '.') {
            // Concatenation
            State* end2 = &nfa->states[--nfa->num_states];
            State* end1 = &nfa->states[--nfa->num_states];
            end1->transitions[26] = nfa->num_states; // epsilon transition
            *end2 = *end1;
            nfa->num_states++;
        } else if (c == '|') {
            // Union
            State new_start = {.is_final = false};
            State new_end = {.is_final = true};
            State* end2 = &nfa->states[--nfa->num_states];
            State* start2 = &nfa->states[--nfa->num_states];
            State* end1 = &nfa->states[--nfa->num_states];
            State* start1 = &nfa->states[--nfa->num_states];
            new_start.transitions[26] = nfa->num_states + 1; // epsilon to start1
            new_start.transitions[27] = nfa->num_states + 3; // epsilon to start2
            end1->transitions[26] = nfa->num_states + 5; // epsilon to new_end
            end2->transitions[26] = nfa->num_states + 5; // epsilon to new_end
            nfa->states[nfa->num_states++] = new_start;
            nfa->states[nfa->num_states++] = *start1;
            nfa->states[nfa->num_states++] = *end1;
            nfa->states[nfa->num_states++] = *start2;
            nfa->states[nfa->num_states++] = *end2;
            nfa->states[nfa->num_states++] = new_end;
        } else if (c == '*') {
            // Kleene star
            State new_start = {.is_final = false};
            State new_end = {.is_final = true};
            State* end = &nfa->states[--nfa->num_states];
            State* start = &nfa->states[--nfa->num_states];
            new_start.transitions[26] = nfa->num_states + 1; // epsilon to start
            new_start.transitions[27] = nfa->num_states + 3; // epsilon to new_end
            end->transitions[26] = nfa->num_states + 1; // epsilon to start
            end->transitions[27] = nfa->num_states + 3; // epsilon to new_end
            nfa->states[nfa->num_states++] = new_start;
            nfa->states[nfa->num_states++] = *start;
            nfa->states[nfa->num_states++] = *end;
            nfa->states[nfa->num_states++] = new_end;
        }
    }

    return nfa;
}

DFA* nfa_to_dfa(DFA* nfa) {
    DFA* dfa = malloc(sizeof(DFA));
    dfa->num_states = 0;
    dfa->start_state = 0;
    memcpy(dfa->alphabet, nfa->alphabet, sizeof(nfa->alphabet));
    dfa->alphabet_size = nfa->alphabet_size;

    bool visited[MAX_STATES][MAX_STATES] = {false};
    int queue[MAX_STATES][MAX_STATES], front = 0, rear = 0;

    // Start state
    queue[rear][0] = nfa->start_state;
    queue[rear][1] = -1;
    rear++;

    while (front != rear) {
        int* current_set = queue[front++];
        int dfa_state = dfa->num_states++;
        dfa->states[dfa_state].is_final = false;

        for (int i = 0; current_set[i] != -1; i++) {
            if (nfa->states[current_set[i]].is_final) {
                dfa->states[dfa_state].is_final = true;
                break;
            }
        }

        for (int symbol = 0; symbol < 26; symbol++) {
            if (!nfa->alphabet[symbol]) continue;

            int next_set[MAX_STATES], next_size = 0;
            for (int i = 0; current_set[i] != -1; i++) {
                int next = nfa->states[current_set[i]].transitions[symbol];
                if (next != 0 && !visited[dfa_state][next]) {
                    next_set[next_size++] = next;
                    visited[dfa_state][next] = true;
                }
            }

            if (next_size > 0) {
                next_set[next_size] = -1;
                memcpy(queue[rear], next_set, (next_size + 1) * sizeof(int));
                rear++;
                dfa->states[dfa_state].transitions[symbol] = dfa->num_states;
            }
        }
    }

    return dfa;
}

DFA* minimize_dfa(DFA* dfa) {
    bool equivalent[MAX_STATES][MAX_STATES] = {false};
    
    // Initialize: mark pairs of states, one final and one non-final, as distinguishable
    for (int i = 0; i < dfa->num_states; i++) {
        for (int j = i + 1; j < dfa->num_states; j++) {
            if (dfa->states[i].is_final != dfa->states[j].is_final) {
                equivalent[i][j] = equivalent[j][i] = true;
            }
        }
    }

    // Find distinguishable states
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < dfa->num_states; i++) {
            for (int j = i + 1; j < dfa->num_states; j++) {
                if (!equivalent[i][j]) {
                    for (int symbol = 0; symbol < 26; symbol++) {
                        if (!dfa->alphabet[symbol]) continue;
                        int next_i = dfa->states[i].transitions[symbol];
                        int next_j = dfa->states[j].transitions[symbol];
                        if (equivalent[next_i][next_j]) {
                            equivalent[i][j] = equivalent[j][i] = true;
                            changed = true;
                            break;
                        }
                    }
                }
            }
        }
    } while (changed);

    // Construct minimized DFA
    DFA* min_dfa = malloc(sizeof(DFA));
    min_dfa->num_states = 0;
    min_dfa->start_state = 0;
    memcpy(min_dfa->alphabet, dfa->alphabet, sizeof(dfa->alphabet));
    min_dfa->alphabet_size = dfa->alphabet_size;

    int state_map[MAX_STATES];
    for (int i = 0; i < dfa->num_states; i++) {
        if (state_map[i] == 0) {
            state_map[i] = min_dfa->num_states++;
            for (int j = i + 1; j < dfa->num_states; j++) {
                if (!equivalent[i][j]) {
                    state_map[j] = state_map[i];
                }
            }
        }
    }

    for (int i = 0; i < dfa->num_states; i++) {
        int new_state = state_map[i];
        min_dfa->states[new_state].is_final = dfa->states[i].is_final;
        for (int symbol = 0; symbol < 26; symbol++) {
            if (!dfa->alphabet[symbol]) continue;
            int next = dfa->states[i].transitions[symbol];
            min_dfa->states[new_state].transitions[symbol] = state_map[next];
        }
    }

    return min_dfa;
}

void print_dfa(DFA* dfa) {
    printf("Number of states: %d\n", dfa->num_states);
    printf("Start state: %d\n", dfa->start_state);
    printf("Alphabet: ");
    for (int i = 0; i < 26; i++) {
        if (dfa->alphabet[i]) printf("%c ", 'a' + i);
    }
    printf("\n");
    for (int i = 0; i < dfa->num_states; i++) {
        printf("State %d (Final: %s):\n", i, dfa->states[i].is_final ? "Yes" : "No");
        for (int j = 0; j < 26; j++) {
            if (dfa->alphabet[j] && dfa->states[i].transitions[j] != 0) {
                printf("  %c -> %d\n", 'a' + j, dfa->states[i].transitions[j]);
            }
        }
    }
}

int main() {
    char re[] = "(a|b)*abb";
    char postfix[100];
    
    printf("Regular Expression: %s\n", re);
    
    re_to_postfix(re, postfix);
    printf("Postfix: %s\n", postfix);

    DFA* nfa = build_nfa(postfix);
    printf("\nNFA:\n");
    print_dfa(nfa);

    DFA* dfa = nfa_to_dfa(nfa);
    printf("\nDFA:\n");
    print_dfa(dfa);

    DFA* min_dfa = minimize_dfa(dfa);
    printf("\nMinimized DFA:\n");
    print_dfa(min_dfa);

    free(nfa);
    free(dfa);
    free(min_dfa);

    return 0;
}