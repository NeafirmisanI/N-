#include <iostream>
#include "mainClasses.hpp"

using namespace std;

void Parser::parse() {
    
}

void Parser::update_tok() {
    current_tok = (token_idx >= 0 && token_idx < tokens.size()) ? &(tokens[token_idx]) : new Token("\0", "\0", -2, -2);
}

Token Parser::advance() {
    ++token_idx;
    update_tok();
    return *current_tok;
}
