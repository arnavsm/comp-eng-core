#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_VARS 26 // For A-Z and a-z excluding t and f

typedef enum { T, F, VAR, AND, OR, NOT, IMPLIES, IFF, LPAREN, RPAREN, END, ERROR } TokenType;

typedef struct {
    TokenType type;
    char value;
} Token;

Token tokens[MAX_TOKENS];
int token_count = 0;
char variables[MAX_VARS];
int var_count = 0;

// Function to tokenize the input string
void tokenize(const char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (isupper(input[i]) || islower(input[i])) {
            if (input[i] != 't' && input[i] != 'f') {
                tokens[token_count++] = (Token){.type = VAR, .value = input[i]};
                if (strchr(variables, input[i]) == NULL) {
                    variables[var_count++] = input[i];
                }
            } else {
                tokens[token_count++] = (Token){.type = (input[i] == 't') ? T : F, .value = input[i]};
            }
        } else if (input[i] == '/' && input[i + 1] == '\\') {
            tokens[token_count++] = (Token){.type = AND, .value = 0}; // AND OR
            i++; // Skip the next character
        } else if (input[i] == '~') {
            tokens[token_count++] = (Token){.type = NOT, .value = 0};
        } else if (input[i] == '-' && input[i + 1] == '>') {
            tokens[token_count++] = (Token){.type = IMPLIES, .value = 0};
            i++; // Skip the next character
        } else if (input[i] == '<' && input[i + 1] == '-' && input[i + 2] == '>') {
            tokens[token_count++] = (Token){.type = IFF, .value = 0};
            i += 2; // Skip the next two characters
        } else if (input[i] == '(') {
            tokens[token_count++] = (Token){.type = LPAREN, .value = 0};
        } else if (input[i] == ')') {
            tokens[token_count++] = (Token){.type = RPAREN, .value = 0};
        } else if (isspace(input[i])) {
            continue; // Ignore whitespace
        } else {
            printf("Lexical Error at position %d: Invalid token '%c'\n", i, input[i]);
            exit(1);
        }
    }
    tokens[token_count++] = (Token){.type = END, .value = 0};

    // Debug: Print tokens
    printf("Tokens:\n");
    for (int j = 0; j < token_count; j++) {
        printf("Token %d: Type %d, Value '%c'\n", j, tokens[j].type, tokens[j].value);
    }
}

// Parsing functions
void parse_expression(int *pos);
void parse_term(int *pos);
void parse_factor(int *pos);
void parse_primary(int *pos);

void parse_expression(int *pos) {
    parse_term(pos);
    while (tokens[*pos].type == OR) {
        (*pos)++;
        parse_term(pos);
    }
}

void parse_term(int *pos) {
    parse_factor(pos);
    while (tokens[*pos].type == AND) {
        (*pos)++;
        parse_factor(pos);
    }
}

void parse_factor(int *pos) {
    if (tokens[*pos].type == NOT) {
        (*pos)++;
        parse_factor(pos);
    } else {
        parse_primary(pos);
    }
}

void parse_primary(int *pos) {
    if (tokens[*pos].type == LPAREN) {
        (*pos)++;
        parse_expression(pos);
        if (tokens[*pos].type != RPAREN) {
            printf("Syntax Error: Missing ')'\n");
            exit(1);
        }
        (*pos)++;
    } else if (tokens[*pos].type == T || tokens[*pos].type == F || tokens[*pos].type == VAR) {
        (*pos)++;
    } else {
        printf("Syntax Error: Unexpected token\n");
        exit(1);
    }
}

void check_syntax() {
    int pos = 0;
    parse_expression(&pos);
    if (tokens[pos].type != END) {
        printf("Syntax Error: Unexpected token at position %d\n", pos);
        exit(1);
    }

    int open_parentheses = 0;

    for (int i = 0; i < pos; i++) {
        if (tokens[i].type == LPAREN) {
            open_parentheses++;
        } else if (tokens[i].type == RPAREN) {
            open_parentheses--;
        }

        if (open_parentheses < 0) {
            printf("Syntax Error: Unmatched closing parenthesis at position %d\n", i);
            exit(1);
        }
    }

    if (open_parentheses > 0) {
        printf("Syntax Error: Unmatched opening parenthesis\n");
        exit(1);
    }
}

int main() {
    char input[256];
    printf("Enter a propositional logic expression: ");
    fgets(input, sizeof(input), stdin);

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    tokenize(input);
    check_syntax();

    printf("Expression is valid!\n");

    return 0;
}
