#include <iostream>
#include <vector>
#include <tuple>
#include "token.hpp"
#include "error.hpp"

using namespace std;

class Lexer {
    public:
        string code;
        char current_char;
        Position* pos;
        Position* noPos;
        tuple<vector<Token>, Error> lex();
        void advance();
        void skip_comment();
        Token parse_number();
        Token parse_identifier();
        Token parse_string();
        tuple<Token, Error> parse_not_equals();
        Token parse_equals();
        Token parse_less_than();
        Token parse_greater_than();
        bool in(char, string);
        bool in(string, vector<string>);

        Lexer(string text) {
            code = text;
            pos = new Position(-1, 0, -1, "<stdin>", text);
            noPos = new Position(-2, -2, -2, "", "");
            current_char = '\0';
            advance();
        }
};

class Parser {
    public:
        int token_idx;
        Token* current_tok;
        vector<Token> tokens;
        void parse();
        void update_tok();
        Token advance();

        Parser(vector<Token> toks) {
            current_tok = new Token("\0", "\0", -2, -2);
            tokens = toks;
            token_idx = -1;
            advance();
        }
};
