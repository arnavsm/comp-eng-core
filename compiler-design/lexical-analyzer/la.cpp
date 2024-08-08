#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>

using namespace std;

// Token types
enum class TokenType {
    Identifier, Keyword, Operator, Separator, Literal, EOF_Token
};

// Token struct
struct Token {
    TokenType type;
    string lexeme;
    int lineNumber;
};

// Keywords
const vector<string> keywords = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

// Operators
const vector<string> operators = {
    "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!", "&", "|", "^", "~", "<<", ">>"
};

// Separators
const vector<string> separators = {
    "{", "}", "[", "]", "(", ")", ";", ",", "."
};

// Classify a token
TokenType classifyToken(const string& lexeme) {
    // Check if the lexeme is a keyword
    for (const auto& keyword : keywords) {
        if (lexeme == keyword) {
            return TokenType::Keyword;
        }
    }

    // Check if the lexeme is an operator
    for (const auto& op : operators) {
        if (lexeme == op) {
            return TokenType::Operator;
        }
    }

    // Check if the lexeme is a separator
    for (const auto& sep : separators) {
        if (lexeme == sep) {
            return TokenType::Separator;
        }
    }

    // Check if the lexeme is a literal
    if (isdigit(lexeme[0]) || (lexeme[0] == '-' && isdigit(lexeme[1]))) {
        return TokenType::Literal;
    }

    // If none of the above, it's an identifier
    return TokenType::Identifier;
}

// Scan the input file and generate tokens
vector<Token> scanInput(const string& filename) {
    vector<Token> tokens;
    ifstream file(filename);
    string line;
    int lineNumber = 1;

    if (file.is_open()) {
        while (getline(file, line)) {
            string currentLexeme = "";
            for (char c : line) {
                if (isalnum(c) || c == '_') {
                    currentLexeme += c;
                } else {
                    if (!currentLexeme.empty()) {
                        tokens.push_back({classifyToken(currentLexeme), currentLexeme, lineNumber});
                        currentLexeme = "";
                    }

                    if (isspace(c)) {
                        if (c == '\n') {
                            lineNumber++;
                        }
                    } else {
                        string charStr(1, c);
                        tokens.push_back({classifyToken(charStr), charStr, lineNumber});
                    }
                }
            }

            if (!currentLexeme.empty()) {
                tokens.push_back({classifyToken(currentLexeme), currentLexeme, lineNumber});
            }
        }

        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }

    tokens.push_back({TokenType::EOF_Token, "", lineNumber});
    return tokens;
}

int main() {
    vector<Token> tokens = scanInput("example.cpp");

    for (const auto& token : tokens) {
        cout << "Token Type: " << static_cast<int>(token.type) << ", Lexeme: " << token.lexeme << ", Line: " << token.lineNumber << endl;
    }

    return 0;
}