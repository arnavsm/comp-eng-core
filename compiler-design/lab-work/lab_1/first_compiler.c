#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// not to be done like this
// bool checkHeader(const char* line) {
//     // extract the first token
//     char* token = strtok(line, " ");
//     char* first = token;
//     // loop through the string to extract all other tokens
//     while(token != NULL) {
//        printf(" %s\n", token); //printing each token
//        token = strtok(NULL," ");
//     }

//     char* second = strtok(NULL," ");
//     char* third = strtok(NULL," ");

//     if (!strtok(NULL," ")) {
//         printf("Extra Tokens");
//     }
// }

int main(int argc, char* argv[])
{
    // taking arguments
    printf("You have entered %d arguments:\n", argc);
 
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    // opening file
    char file_name[7]; // Specify a fixed size
    strcpy(file_name, argv[1]);

    FILE* fptr;
    fptr = fopen(file_name, "r");
    if (!fptr) {
        printf("The file is not opened. The program will now exit.");
        exit(0);
    }

    // reading from file
    char line[1000];

    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = '\0'; // removing newline character
        
        // not to be done like this
        if (strstr(line, "Begin_Header") && strstr(line, "End_Header")) {
        } else if (strstr(line, "Body{")) {
        } else if (strstr(line, "Results{")) {
        }
    }
    fclose(fptr);
}