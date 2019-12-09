#include <iostream>
#include "mainClasses.hpp"

void Parser::parse() {
    ParseResult* res = new ParseResult();
    std::vector<Node> statements;
    Position* pos_start = new Position(current_tok->pos_start->inx, current_tok->pos_start->ln, current_tok->pos_start->col, current_tok->pos_start->fn, current_tok->pos_start->ftext);
}

void Parser::update_tok() {
    current_tok = (token_idx >= 0 && token_idx < tokens.size()) ? &(tokens[token_idx]) : new Token("\0", "\0", noPos, noPos);
}

Token Parser::reverse(int amount) {
    token_idx -= amount;
    update_tok();
    return *current_tok;
}

Token Parser::advance() {
    ++token_idx;
    update_tok();
    return *current_tok;
}

void ParseResult::register_advancement() {
    last_registered_advance_count = 1;
    ++advance_count;
}

Node* ParseResult::register_(ParseResult res) {
    last_registered_advance_count = res.advance_count;
    advance_count += res.advance_count;
    if (res.error) error = res.error;
    return res.node;
}

Node* ParseResult::try_register(ParseResult res) {
    if (res.error) {
        to_reverse_count = res.advance_count;
        return new Node();
    }
    return register_(res);
}

ParseResult* ParseResult::success(Node* node_) {
    node = node_;
    return this;
}

ParseResult* ParseResult::failure(Error* error_) {
    if (!error || last_registered_advance_count == 0) error = error_;
    return this;
}
