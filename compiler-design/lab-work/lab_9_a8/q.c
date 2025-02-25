#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

char stack[MAX][10]; // Operator stack
int top = -1;        // Stack pointer
int tempCount = 1;   // Temporary variable counter

void push(char *op) {
    strcpy(stack[++top], op);
}

char* pop() {
    return stack[top--];
}

int precedence(char op) {
    switch (op) {
        case '(': return 0;
        case '+':
        case '-': return 1;
        case '*':
        case '/':
        case '%': return 2;
        default: return -1;
    }
}

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '=');
}

int isIdentifier(char ch) {
    return (ch >= 'A' && ch <= 'Z');
}

void generateCode(char *expression) {
    char op1[10], op2[10], result[10];
    char operator;

    char *token = strtok(expression, " ");
    while (token != NULL) {
        if (isIdentifier(token[0])) {
            push(token);
        } else if (isOperator(token[0])) {
            while (top > 0 && precedence(stack[top][0]) >= precedence(token[0])) {
                strcpy(op2, pop()); 
                strcpy(op1, pop()); 
                operator = pop()[0];

                sprintf(result, "t%d", tempCount++);
                printf("%s = %c %s %s\n", result, operator, op1, op2);
                push(result);
            }
            char currentOp[2] = {token[0], '\0'};
            push(currentOp);
        }
        token = strtok(NULL, " ");
    }

    while (top > 1) {
        strcpy(op2, pop()); 
        strcpy(op1, pop()); 
        operator = pop()[0]; 

        sprintf(result, "t%d", tempCount++);
        printf("%s = %c %s %s\n", result, operator, op1, op2);
        push(result); 
    }
}

int main() {
    char expression[MAX];

    printf("Enter an algebraic expression (e.g., A - B + C / D / E):\n");
    fgets(expression, sizeof(expression), stdin);
    
    expression[strcspn(expression, "\n")] = 0;

    printf("Three-address code:\n");
    generateCode(expression);

    return 0;
}
