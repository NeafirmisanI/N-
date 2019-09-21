#include <iostream>

using namespace std;

class Lexer {
    public:
        string code;
        char current_char;
        int pos;
        void lex();
        void advance();
        void skip_comment();
        string charToString(char);
        bool in(char, string);

        Lexer(string text) {
            code = text;
            pos = -1;
            current_char = '\0';
            advance();
        }
};
