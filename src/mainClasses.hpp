#include <algorithm>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <tuple>

class Position {
    public:
        int8_t inx;
        int8_t ln;
        int8_t col;
        std::string fn;
        std::string ftext;
        void advance();

        Position(int8_t index, int8_t line, int8_t column, std::string filename, std::string filetext) {
            inx = index;
            ln = line;
            col = column;
            fn = filename;
            ftext = filetext;
        }
};

class Error {
    private:
        std::string highlight(std::string text, Position* pos_start, Position* pos_end);
    public:
        Position* pos_start;
        Position* pos_end;
        std::string error_name;
        std::string details;
        std::string as_string();

        Error(Position* pos_s, Position* pos_e, std::string error_nam, std::string detail) {
            pos_start = pos_s;
            pos_end = pos_e;
            error_name = error_nam;
            details = detail;
        }
};

class Token {
    public:
        std::string value;
        std::string type;
        Position* pos_start;
        Position* pos_end;
        bool matches(std::string type_, std::string val);
        std::string repr();

        Token(std::string type_, std::string val, Position* pos_strt, Position* pos_nd) {
            type = type_;
            value = val;

            if (pos_strt->inx != -2) {
                pos_start = pos_strt;
                pos_end = pos_start;
            }

            if (pos_nd->inx != -2) {
                pos_end = pos_nd;
            }
        }
};

class Lexer {
    private:
        std::string code;
        Position* pos;
        Position* noPos;
        void advance();
        void skip_comment();
        Token parse_number();
        Token parse_identifier();
        Token parse_string();
        std::tuple<Token, Error> parse_not_equals();
        Token parse_equals();
        Token parse_less_than();
        Token parse_greater_than();
        Token make_token(std::string type_);
        bool is_digit(char c);
        bool is_identifier(char c);
        bool in(char, std::string);
        bool in(std::string, std::vector<std::string>);
        char current_char = '\0';
    public:
        std::tuple<std::vector<Token>, Error> lex();

        Lexer(std::string text) {
            noPos = new Position(-2, -2, -2, "", "");
            pos = new Position(-1, 0, -1, "<stdin>", text);
            code = text;
            advance();
        }
};

class Parser {
    private:
        int8_t token_idx = -1;
        Token* no_tok;
        Token* current_tok;
        Position* noPos;
        std::vector<Token> tokens;
        void update_tok();
        Token reverse(int amount);
        Token advance();
    public:
        void parse();

        Parser(std::vector<Token> toks) {
            noPos = new Position(-2, -2, -2, "", "");
            tokens = toks;
            advance();
        }
};

class Node {
    public:
        Node() {

        }
};

class ParseResult {
    private:
        Position* noPos;
        int8_t last_registered_advance_count = 0;
        int8_t advance_count = 0;
        int8_t to_reverse_count = 0;
    public:
        Error* error;
        Node* node;
        void register_advancement();
        Node* register_(ParseResult res);
        Node* try_register(ParseResult res);
        ParseResult* success(Node* node_);
        ParseResult* failure(Error* error);

        ParseResult() {
            noPos = new Position(-2, -2, -2, "", "");
        }
};
