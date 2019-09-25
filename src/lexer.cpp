#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include "lexer.hpp"
#include "token.hpp"

using namespace std;

static string digits = "0123456789";
static string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static vector<string> KEYWORDS = {"var", "and", "or", "not", "if", "elif", "else", "for", "to", "step", "while", "def", "then", "end", "return", "continue", "break"};

void Lexer::lex() {
    vector<string> tokens;

    while (current_char != '\0') {
        if (in(current_char, " \t")) {
            advance();
        } else if (current_char == '#') {
            skip_comment();
        } else if (in(current_char, "\n;")) {
            tokens.push_back(Token("NEWLINE", "", pos, -2).repr());
            advance();
        } else if (in(current_char, digits)) {
            tokens.push_back(parse_number());
        } else if (in(current_char, letters)) {
            tokens.push_back(parse_identifier());
        } else if (current_char == '\"') {
            tokens.push_back(parse_string());
        } else if (current_char == '+') {
            tokens.push_back(Token("PLUS", '\0', pos, -2).repr());
            advance();
        } else if (current_char == '-') {
            tokens.push_back(Token("MINUS", '\0', pos, -2).repr());
            advance();
        } else if (current_char == '*') {
            tokens.push_back(Token("MUL", '\0', pos, -2).repr());
            advance();
        } else if (current_char == '/') {
            tokens.push_back(Token("DIV", '\0', pos, -2).repr());
            advance();
        } else if (current_char == '%') {
            tokens.push_back(Token("MOD", '\0', pos, -2).repr());
            advance();
        } else if (current_char == '^') {
            tokens.push_back(Token("POW", '\0', pos, -2).repr());
            advance();
        } else if (current_char == '(') {
            tokens.push_back(Token("LPAREN", '\0', pos, -2).repr());
            advance();
        } else if (current_char == ')') {
            tokens.push_back(Token("RPAREN", '\0', pos, -2).repr());
            advance();
        } else if (current_char == '[') {
            tokens.push_back(Token("LSQARE", '\0', pos, -2).repr());
            advance();
        } else if (current_char == ']') {
            tokens.push_back(Token("RSQARE", '\0', pos, -2).repr());
            advance();
        } else if (current_char == '!') {
            
        }
    }

    for (string n : tokens) // Output the contents for verification
        cout << n << ", ";
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

string Lexer::parse_number() {
    string num_str = "";
    bool dot = false;
    int pos_start = pos;

    while (current_char != '\0' && in(current_char, digits + ".")) {
        if (current_char == '.') {
            if (dot) break;
            dot = true;
        }

        num_str += current_char;
        advance();
    }

    if (dot) {
        return Token("FLOAT", num_str, pos_start, pos).repr();
    }
    return Token("INT", num_str, pos_start, pos).repr();
}

string Lexer::parse_identifier() {
    string id_str = "";
    int pos_start = pos;

    while (current_char != '\0' && in(current_char, letters + digits + "_")) {
        id_str += current_char;
        advance();
    }

    string tok_type = (in(id_str, KEYWORDS)) ? "KEYWORD" : "IDENTIFIER";
    return Token(tok_type, id_str, pos_start, pos).repr();
}

string Lexer::parse_string() {
    string str = "";
    int pos_start = pos;
    bool escape_char = false;
    advance();

    map<char, char> escape_characters;
    escape_characters['n'] = '\n';
    escape_characters['t'] = '\t';

    while (current_char != '\0' && (current_char != '\"' || escape_char)) {
        if (escape_char) {
            str += (escape_characters[current_char]) ? escape_characters[current_char] : current_char;
        } else {
            if (current_char == '\\') {
                escape_char = true;
            } else {
                str += current_char;
            }
        }

        advance();
        escape_char = false;
    }

    advance();
    return Token("STRING", str, pos_start, pos).repr();
}

bool Lexer::in(char c, string str) {
    return str.find(c) != string::npos;
}

bool Lexer::in(string str, vector<string> array) {
    return find(array.begin(), array.end(), str) != array.end();
}

string Lexer::charToString(char c) {
    string str(1, c);
    return str;
}
