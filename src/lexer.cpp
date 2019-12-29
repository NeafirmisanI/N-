#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "mainClasses.hpp"

static std::string digits = "0123456789";
static std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static std::vector<std::string> KEYWORDS = {"var", "and", "or", "not", "if", "elif", "else", "for", "to", "step", "while", "def", "then", "end", "return", "continue", "break"};

std::tuple<std::vector<Token*>, Error> Lexer::lex() {
    std::vector<Token*> tokens;

    while (current_char != '\0') {
        if (is_identifier(current_char)) tokens.push_back(parse_identifier());
        if (current_char == '\"') tokens.push_back(parse_string());
        if (is_digit(current_char)) tokens.push_back(parse_number());
        
        switch (current_char) {
            case '\0': break;
            case '\t':
            case ' ': advance(); break;
            case '\n':
            case ';': tokens.push_back(make_token("NEWLINE")); break;
            case '+': tokens.push_back(make_token("PLUS")); break;
            case '-': tokens.push_back(parse_minus()); break;
            case '*': tokens.push_back(make_token("MUL")); break;
            case '/': tokens.push_back(make_token("DIV")); break;
            case '%': tokens.push_back(make_token("MOD")); break;
            case '^': tokens.push_back(make_token("POW")); break;
            case '(': tokens.push_back(make_token("LPAREN")); break;
            case ')': tokens.push_back(make_token("RPAREN")); break;
            case '[': tokens.push_back(make_token("LSQUARE")); break;
            case ']': tokens.push_back(make_token("RSQUARE")); break;
            case ',': tokens.push_back(make_token("COMMA")); break;
            case '=': tokens.push_back(parse_equals()); break;
            case '<': tokens.push_back(parse_less_than()); break;
            case '>': tokens.push_back(parse_greater_than()); break;
            case '#': skip_comment(); break;
            case '!': {
                    std::tuple<Token*, Error> res = parse_not_equals();

                    if (std::get<1>(res).details != "") {
                        std::tuple<std::vector<Token*>, Error> eres({noTok}, std::get<1>(res));
                        return eres;
                    }

                    tokens.push_back(std::get<0>(res));
                    break;
            } default: {
                    Position* pos_start = pos->copy();
                    std::string sc(1, current_char);
                    advance();
                    std::tuple<std::vector<Token*>, Error> eres({noTok}, Error(pos_start, pos, "Illegal Character", "\'" + sc + "\'"));
                    return eres;
            }
        }
    }

    Position* pos_start = pos->copy();
    advance();
    tokens.push_back(new Token("EOF", "\0", pos_start, pos));
    return make_tuple(tokens, Error(noPos, noPos, "", ""));
}

void Lexer::advance() {
    pos->advance();
    current_char = (pos->inx < code.size()) ? code[pos->inx] : '\0';
}

void Lexer::skip_comment() {
    advance();
    while (!in(current_char, "#\n") && current_char != '\0') advance();
    advance();
}

Token* Lexer::make_token(std::string type_) {
    Position* pos_start = pos->copy();
    advance();
    return new Token(type_, "\0", pos_start, pos);
}

Token* Lexer::parse_minus() {
    std::string tok_type = "MINUS";
    Position* pos_start = pos->copy();
    advance();

    if (current_char == '>') {
        advance();
        tok_type = "ARROW";
    }

    return new Token(tok_type, "\0", pos_start, pos);
}

std::tuple<Token*, Error> Lexer::parse_not_equals() {
    Position* pos_start = pos->copy();
    advance();

    if (current_char == '=') {
        advance();
        std::tuple<Token*, Error> res(new Token("NE", "\0", pos_start, pos), Error(noPos, noPos, "", ""));
        return res;
    }

    advance();
    std::tuple<Token*, Error> res(new Token("\0", "\0", noPos, noPos), Error(pos_start, pos, "Expected Character", "'=' (after '!')"));
    return res;
}

Token* Lexer::parse_equals() {
    std::string tok_type = "EQ";
    Position* pos_start = pos->copy();
    advance();

    if (current_char == '=') {
        advance();
        tok_type = "EE";
    }

    return new Token(tok_type, "\0", pos_start, pos);
}

Token* Lexer::parse_less_than() {
    std::string tok_type = "LT";
    Position* pos_start = pos->copy();
    advance();

    if (current_char == '=') {
        advance();
        tok_type = "LTE";
    }

    return new Token(tok_type, "\0", pos_start, pos);
}

Token* Lexer::parse_greater_than() {
    std::string tok_type = "GT";
    Position* pos_start = pos->copy();
    advance();

    if (current_char == '=') {
        advance();
        tok_type = "GTE";
    }

    return new Token(tok_type, "\0", pos_start, pos);
}

Token* Lexer::parse_number() {
    std::string num_str = "";
    bool dot = false;
    Position* pos_start = pos->copy();

    while (is_digit(current_char) || current_char == '.') {
        if (current_char == '.') {
            if (dot) break;
            dot = true;
        }

        num_str += current_char;
        advance();
    }

    if (dot) return new Token("FLOAT", num_str, pos_start, pos);
    return new Token("INT", num_str, pos_start, pos);
}

Token* Lexer::parse_identifier() {
    std::string id_str = "";
    Position* pos_start = pos->copy();

    while (current_char != '\0' && (is_identifier(current_char) || is_digit(current_char))) {
        id_str += current_char;
        advance();
    }

    std::string tok_type = (in(id_str, KEYWORDS)) ? "KEYWORD" : "IDENTIFIER";
    return new Token(tok_type, id_str, pos_start, pos);
}

Token* Lexer::parse_string() {
    std::string str = "";
    Position* pos_start = pos->copy();
    bool escape_char = false;
    advance();

    std::map<char, char> escape_characters;
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
    return new Token("STRING", str, pos_start, pos);
}

bool Lexer::is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::is_identifier(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Token::matches(std::string type_, std::string val) {
    return (type == type_) && (value == val);
}

std::string Token::repr() {
    if (value != "\0") return type + ":" + value;
    return type;
}
