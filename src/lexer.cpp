// This is the alpha stage for the N# lexer

#include <iostream>
#include <vector>
#include "lexer.hpp"

using namespace std;

void Lexer::lex() {
    vector<string> tokens;

    while (current_char != '\0') {
        if (in(current_char, " \t")) {
            advance();
        } else if (current_char == '#') {
            skip_comment();
        } else if (in(current_char, "\n;")) {
            tokens.push_back("[NEWLINE]");
            advance();
        } else {
            tokens.push_back(charToString(current_char));
            advance();
        }
    }

    for (string n : tokens) // Output the contents for verification
        cout << n;
}

void Lexer::advance() {
    ++pos;
    current_char = (pos < code.size()) ? code[pos] : '\0';
}

void Lexer::skip_comment() {
    advance();

    while (!in(current_char, "#\n")) {
        advance();
    }

    advance();
}

bool Lexer::in(char c, string str) {
    return str.find(c) != string::npos;
}

string Lexer::charToString(char c) {
    string str(1, c);
    return str;
}
