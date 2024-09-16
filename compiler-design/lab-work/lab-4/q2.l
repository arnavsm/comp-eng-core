%{
#include <stdio.h>
#include <string.h>

#define MAX_CODE_LENGTH 10

char current_code[MAX_CODE_LENGTH + 1];
int code_length = 0;

const char *morse_to_char(const char *code) {
    static const char *morse_codes[] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-",
        ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-",
        ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-",
        ".....", "-....", "--...", "---..", "----.", ".-.-.-", "--..--", "..--..",
        ".----.", "-.-.--", "-..-.", "-.--.", "-.--.-", ".-...", "---...", "-.-.-.",
        "-...-", ".-.-.", "-....-", "..--.-", ".-..-.", ".--.-", ""
    };

    static const char *morse_chars[] = {
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
        "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3",
        "4", "5", "6", "7", "8", "9", ".", ",", "?", "'", "!", "/", "(", ")", "&",
        ":", ";", "=", "+", "-", "_", "\"", "@", ""
    };

    for (int i = 0; i < sizeof(morse_codes)/sizeof(morse_codes[0]); i++) {
        if (strcmp(code, morse_codes[i]) == 0) {
            return morse_chars[i];
        }
    }
    return "UNKNOWN";
}

int yywrap(void) {
    return 1;
}
%}

%%
[.-]        { if (code_length < MAX_CODE_LENGTH) current_code[code_length++] = yytext[0]; }
[ ]         { current_code[code_length] = '\0'; printf("%s", morse_to_char(current_code)); code_length = 0; }
\n          { if (code_length > 0) { current_code[code_length] = '\0'; printf("%s", morse_to_char(current_code)); } printf("\n"); code_length = 0; }
.           {  }
%%

int main() {
    yylex();
    return 0;
}



