#include <iostream>
#include <vector>
#include <tuple>
#include "token.hpp"

using namespace std;

class Lexer {
    public:
        string code;
        char current_char;
        int pos;
        tuple<vector<Token>, string> lex();
        void advance();
        void skip_comment();
        Token parse_number();
        Token parse_identifier();
        Token parse_string();
        tuple<Token, string> parse_not_equals();
        Token parse_equals();
        Token parse_less_than();
        Token parse_greater_than();
        bool in(char, string);
        bool in(string, vector<string>);

        Lexer(string text) {
            code = text;
            pos = -1;
            current_char = '\0';
            advance();
        }
};
