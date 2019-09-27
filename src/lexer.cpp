#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "lexer.hpp"

using namespace std;

static string digits = "0123456789";
static string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static vector<string> KEYWORDS = {"var", "and", "or", "not", "if", "elif", "else", "for", "to", "step", "while", "def", "then", "end", "return", "continue", "break"};

tuple<vector<Token>, string> Lexer::lex() {
    vector<Token> tokens;

    while (current_char != '\0') {
        if (in(current_char, " \t")) {
            advance();
        } else if (current_char == '#') {
            skip_comment();
        } else if (in(current_char, "\n;")) {
            tokens.push_back(Token("NEWLINE", "", pos, -2));
            advance();
        } else if (in(current_char, digits)) {
            tokens.push_back(parse_number());
        } else if (in(current_char, letters)) {
            tokens.push_back(parse_identifier());
        } else if (current_char == '\"') {
            tokens.push_back(parse_string());
        } else if (current_char == '+') {
            tokens.push_back(Token("PLUS", "\0", pos, -2));
            advance();
        } else if (current_char == '-') {
            tokens.push_back(Token("MINUS", "\0", pos, -2));
            advance();
        } else if (current_char == '*') {
            tokens.push_back(Token("MUL", "\0", pos, -2));
            advance();
        } else if (current_char == '/') {
            tokens.push_back(Token("DIV", "\0", pos, -2));
            advance();
        } else if (current_char == '%') {
            tokens.push_back(Token("MOD", "\0", pos, -2));
            advance();
        } else if (current_char == '^') {
            tokens.push_back(Token("POW", "\0", pos, -2));
            advance();
        } else if (current_char == '(') {
            tokens.push_back(Token("LPAREN", "\0", pos, -2));
            advance();
        } else if (current_char == ')') {
            tokens.push_back(Token("RPAREN", "\0", pos, -2));
            advance();
        } else if (current_char == '[') {
            tokens.push_back(Token("LSQARE", "\0", pos, -2));
            advance();
        } else if (current_char == ']') {
            tokens.push_back(Token("RSQARE", "\0", pos, -2));
            advance();
        } else if (current_char == '!') {
            tuple<Token, string> res = parse_not_equals();

            if (get<1>(res) != "") {
                tuple<vector<Token>, string> eres({Token("\0", "\0", -2, -2)}, get<1>(res));
                return eres;
            }

            tokens.push_back(get<0>(res));
        } else if (current_char == '=') {
            tokens.push_back(parse_equals());
        } else if (current_char == '<') {
            tokens.push_back(parse_less_than());
        } else if (current_char == '>') {
            tokens.push_back(parse_greater_than());
        } else if (current_char == ',') {
            tokens.push_back(Token("COMMA", "\0", pos, -2));
        } else {
            int pos_start = pos;
            char c = current_char;
            string sc(1, c);
            advance();
            tuple<vector<Token>, string> eres({Token("\0", "\0", -2, -2)}, "\'" + sc + "\'\n");
            return eres;
        }
    }

    tokens.push_back(Token("EOF", "\0", pos, -2));
    return make_tuple(tokens, "");
}

void Lexer::advance() {
    ++pos;
    current_char = (pos < code.size()) ? code[pos] : '\0';
}

void Lexer::skip_comment() {
    advance();

    while (!in(current_char, "#\n") && current_char != '\0') {
        advance();
    }

    advance();
}

tuple<Token, string> Lexer::parse_not_equals() {
    int pos_start = pos;
    advance();

    if (current_char == '=') {
        advance();
        tuple<Token, string> res(Token("NE", "\0", pos_start, pos), "");
        return res;
    }

    advance();
    tuple<Token, string> res(Token("\0", "\0", -2, -2), "'=' (after '!')\n");
    return res;
}

Token Lexer::parse_equals() {
    string tok_type = "EQ";
    int pos_start = pos;
    advance();

    if (current_char == '=') {
        advance();
        tok_type = "EE";
    }

    return Token(tok_type, "\0", pos_start, pos);
}

Token Lexer::parse_less_than() {
    string tok_type = "LT";
    int pos_start = pos;
    advance();

    if (current_char == '=') {
        advance();
        tok_type = "LTE";
    }

    return Token(tok_type, "\0", pos_start, pos);
}

Token Lexer::parse_greater_than() {
    string tok_type = "GT";
    int pos_start = pos;
    advance();

    if (current_char == '=') {
        advance();
        tok_type = "GTE";
    }

    return Token(tok_type, "\0", pos_start, pos);
}

Token Lexer::parse_number() {
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
        return Token("FLOAT", num_str, pos_start, pos);
    }

    return Token("INT", num_str, pos_start, pos);
}

Token Lexer::parse_identifier() {
    string id_str = "";
    int pos_start = pos;

    while (current_char != '\0' && in(current_char, letters + digits + "_")) {
        id_str += current_char;
        advance();
    }

    string tok_type = (in(id_str, KEYWORDS)) ? "KEYWORD" : "IDENTIFIER";
    return Token(tok_type, id_str, pos_start, pos);
}

Token Lexer::parse_string() {
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
    return Token("STRING", str, pos_start, pos);
}

bool Lexer::in(char c, string str) {
    return str.find(c) != string::npos;
}

bool Lexer::in(string str, vector<string> array) {
    return find(array.begin(), array.end(), str) != array.end();
}
