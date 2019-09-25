#include <iostream>
#include <vector>

using namespace std;

class Lexer {
    public:
        string code;
        char current_char;
        int pos;
        void lex();
        void advance();
        void skip_comment();
        string parse_number();
        string parse_identifier();
        string parse_string();
        string charToString(char);
        bool in(char, string);
        bool in(string, vector<string>);

        Lexer(string text) {
            code = text;
            pos = -1;
            current_char = '\0';
            advance();
        }
};
