#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000

int validate_header(char *line) {
    char *start = "Begin_Header";
    char *end = "End_Header";
    
    if (strncmp(line, start, strlen(start)) != 0) return 0;
    if (strstr(line, end) == NULL) return 0;
    
    char *header_name = line + strlen(start);
    while (isspace(*header_name)) header_name++;
    
    if (*header_name == '\0' || *header_name == ' ') return 0;
    
    return 1;
}

int validate_body(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int arithmetic_op = 0, logic_op = 0;
    
    if (fgets(line, sizeof(line), file) == NULL || strcmp(line, "Body{\n") != 0) return 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strcmp(line, "}\n") == 0) break;
        
        if (strncmp(line, "ArithmaticOperation:", 20) == 0) {
            arithmetic_op = 1;
            if (strstr(line, "result = operand_1 + operand_2;") == NULL) return 0;
        } else if (strncmp(line, "LogicOperation:", 15) == 0) {
            logic_op = 1;
            if (strstr(line, "Output = input_1 & input_2;") == NULL) return 0;
        } else {
            return 0;
        }
    }
    
    return arithmetic_op && logic_op;
}

int validate_results(FILE *file) {
    char line[MAX_LINE_LENGTH];
    int print_count = 0;
    
    if (fgets(line, sizeof(line), file) == NULL || strcmp(line, "Results{\n") != 0) return 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strcmp(line, "}\n") == 0) break;
        
        if (strncmp(line, "Print(", 6) == 0 && line[strlen(line) - 2] == ';') {
            print_count++;
        } else {
            return 0;
        }
    }
    
    return print_count == 2;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    int is_valid = 1;
    
    // Validate header
    if (fgets(line, sizeof(line), file) == NULL || !validate_header(line)) {
        is_valid = 0;
    }
    
    // Validate body
    if (is_valid && !validate_body(file)) {
        is_valid = 0;
    }
    
    // Validate results
    if (is_valid && !validate_results(file)) {
        is_valid = 0;
    }
    
    fclose(file);
    
    if (is_valid) {
        printf("The input file contains a valid programming language construct.\n");
    } else {
        printf("The input file contains an invalid programming language construct.\n");
    }
    
    return 0;
}