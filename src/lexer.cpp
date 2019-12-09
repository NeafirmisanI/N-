#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "mainClasses.hpp"

static std::string digits = "0123456789";
static std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static std::vector<std::string> KEYWORDS = {"var", "and", "or", "not", "if", "elif", "else", "for", "to", "step", "while", "def", "then", "end", "return", "continue", "break"};

std::tuple<std::vector<Token>, Error> Lexer::lex() {
    std::vector<Token> tokens;

    while (current_char != '\0') {
        if (current_char == '\t' || current_char == ' ') advance();
        if (is_identifier(current_char)) tokens.push_back(parse_identifier());
        if (current_char == '\"') tokens.push_back(parse_string());
        if (is_digit(current_char)) tokens.push_back(parse_number());
        
        switch (current_char) {
            case '\0': break;
            case '\n':
            case ';': tokens.push_back(make_token("NEWLINE")); break;
            case '+': tokens.push_back(make_token("PLUS")); break;
            case '-': tokens.push_back(make_token("MINUS")); break;
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
            case '#': skip_comment();
            case '!': {
                    std::tuple<Token, Error> res = parse_not_equals();

                    if (std::get<1>(res).details != "") {
                        std::tuple<std::vector<Token>, Error> eres({Token("\0", "\0", noPos, noPos)}, std::get<1>(res));
                        return eres;
                    }

                    tokens.push_back(std::get<0>(res));
                    break;
            } default: {
                    Position* pos_start = new Position(pos->inx, pos->ln, pos->col, pos->fn, pos->ftext);
                    std::string sc(1, current_char);
                    advance();
                    std::tuple<std::vector<Token>, Error> eres({Token("\0", "\0", noPos, noPos)}, Error(pos_start, pos, "Illegal Character", "\'" + sc + "\'\n")); //TODO: Make token pos_start and pos_end of type Position*
                    return eres;
            }
        }
    }

    tokens.push_back(Token("EOF", "\0", pos, noPos));
    return make_tuple(tokens, Error(noPos, noPos, "", ""));
}

void Lexer::advance() {
    pos->advance();
    current_char = (pos->inx < code.size()) ? code[pos->inx] : '\0';
}

void Lexer::skip_comment() {
    advance();

    while (!in(current_char, "#\n") && current_char != '\0') {
        advance();
    }

    advance();
}

Token Lexer::make_token(std::string type_) {
    advance();
    return Token(type_, "\0", pos, noPos);
}

std::tuple<Token, Error> Lexer::parse_not_equals() {
    Position *pos_start = new Position(pos->inx, pos->ln, pos->col, pos->fn, pos->ftext);
    advance();

    if (current_char == '=') {
        advance();
        std::tuple<Token, Error> res(Token("NE", "\0", pos_start, pos), Error(noPos, noPos, "", ""));
        return res;
    }

    advance();
    std::tuple<Token, Error> res(Token("\0", "\0", noPos, noPos), Error(pos_start, pos, "Expected Character", "'=' (after '!')\n"));
    return res;
}

Token Lexer::parse_equals() {
    std::string tok_type = "EQ";
    Position* pos_start = pos;
    advance();

    if (current_char == '=') {
        advance();
        tok_type = "EE";
    }

    return Token(tok_type, "\0", pos_start, pos);
}

Token Lexer::parse_less_than() {
    std::string tok_type = "LT";
    Position* pos_start = pos;
    advance();

    if (current_char == '=') {
        advance();
        tok_type = "LTE";
    }

    return Token(tok_type, "\0", pos_start, pos);
}

Token Lexer::parse_greater_than() {
    std::string tok_type = "GT";
    Position* pos_start = pos;
    advance();

    if (current_char == '=') {
        advance();
        tok_type = "GTE";
    }

    return Token(tok_type, "\0", pos_start, pos);
}

Token Lexer::parse_number() {
    std::string num_str = "";
    bool dot = false;
    Position* pos_start = pos;

    while (is_digit(current_char) || current_char == '.') {
        if (current_char == '.') {
            if (dot) break;
            dot = true;
        }

        num_str += current_char;
        advance();
    }

    if (dot) return Token("FLOAT", num_str, pos_start, pos);
    return Token("INT", num_str, pos_start, pos);
}

Token Lexer::parse_identifier() {
    std::string id_str = "";
    Position* pos_start = pos;

    while (current_char != '\0' && (is_identifier(current_char) || is_digit(current_char))) {
        id_str += current_char;
        advance();
    }

    std::string tok_type = (in(id_str, KEYWORDS)) ? "KEYWORD" : "IDENTIFIER";
    return Token(tok_type, id_str, pos_start, pos);
}

Token Lexer::parse_string() {
    std::string str = "";
    Position* pos_start = pos;
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
    return Token("STRING", str, pos_start, pos);
}

bool Lexer::is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::is_identifier(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::in(char c, std::string str) {
    return str.find(c) != std::string::npos;
}

bool Lexer::in(std::string str, std::vector<std::string> array) {
    return find(array.begin(), array.end(), str) != array.end();
}
