#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 100

char input[MAX_INPUT];
int pos = 0;

// Function prototypes
void parseS();
void parseA();
void match(char expected);
void error(const char *message);

void parseS() {
    match('c');   // Expecting 'c'
    parseA();    // Parse A
    match('d');   // Expecting 'd'
}

void parseA() {
    if (strncmp(&input[pos], "ab", 2) == 0) {
        // Match "ab"
        pos += 2;
    } else if (input[pos] == 'a') {
        // Match "a"
        pos++;
    } else {
        error("Expected 'a' or 'ab' in production A");
    }
}

void match(char expected) {
    if (input[pos] == expected) {
        pos++;
    } else {
        char errorMsg[50];
        snprintf(errorMsg, sizeof(errorMsg), "Expected '%c' but found '%c'", expected, input[pos]);
        error(errorMsg);
    }
}

void error(const char *message) {
    printf("Error at position %d: %s\n", pos + 1, message);
    exit(1);
}

void parse() {
    parseS();
    if (input[pos] != '\0') {
        error("Extra characters after valid input");
    }
}

int main() {
    // Test the parser
    printf("Enter the input string: ");
    fgets(input, MAX_INPUT, stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline character

    parse();

    printf("The input string '%s' is valid.\n", input);

    return 0;
}
