#include <iostream>
#include "mainClasses.hpp"

ParseResult* Parser::parse() {
    ParseResult* res = statements();
    if (res->error == nullptr && current_tok->type != "EOF") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Token cannot appear after previous tokens"));
    return res;
}

ParseResult* Parser::statements() {
    std::vector<Node*> statements;
    ParseResult* res = new ParseResult();
    Position* pos_start = current_tok->pos_start->copy();

    while (current_tok->type == "NEWLINE") {
        res->register_advancement();
        advance();
    }

    Node* statement = res->register_(statement_());
    if (res->error != nullptr) return res;
    statements.push_back(statement);

    bool more_statements = true;

    for (;;) {
        int newline_count = 0;

        while (current_tok->type == "NEWLINE") {
            res->register_advancement();
            advance();
            ++newline_count;
        }

        if (newline_count == 0) more_statements = false;
        if (!more_statements) break;

        statement = res->try_register(statement_());
        if ((int) statement->pos_start->inx == -2) {
            reverse(res->to_reverse_count);
            more_statements = false;
            continue;
        }

        statements.push_back(statement);
    }

    return res->success(new ListNode(statements, pos_start, current_tok->pos_start->copy()));
}

ParseResult* Parser::statement_() {
    ParseResult* res = new ParseResult();
    Position* pos_start = current_tok->pos_start->copy();

    if (current_tok->matches("KEYWORD", "return")) {
        res->register_advancement();
        advance();
        Node* expression = res->try_register(expr());
        if (!expression) reverse(res->to_reverse_count);
        return res->success(new ReturnNode(expression, pos_start, current_tok->pos_start->copy()));
    }

    if (current_tok->matches("KEYWORD", "continue")) {
        res->register_advancement();
        advance();
        return res->success(new ContinueNode(pos_start, current_tok->pos_start->copy()));
    }

    if (current_tok->matches("KEYWORD", "break")) {
        res->register_advancement();
        advance();
        return res->success(new BreakNode(pos_start, current_tok->pos_start->copy()));
    }

    Node* expression = res->register_(expr());
    if (res->error != nullptr) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'return', 'continue', 'break', 'var', 'if', 'for', 'while', 'def', int, float, identifier, '+', '-', '(', '[' or 'not'"));
    return res->success(expression);
}

ParseResult* Parser::expr() {
    ParseResult* res = new ParseResult();

    if (current_tok->matches("KEYWORD", "var")) {
        res->register_advancement();
        advance();

        if (current_tok->type != "IDENTIFIER") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected identifier"));

        Token* var_name = current_tok;
        res->register_advancement();
        advance();

        if (current_tok->type != "EQ") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected '='"));

        res->register_advancement();
        advance();

        Node* expression = res->register_(expr());
        if (res->error != nullptr) return res;
        return res->success(new VarAssignNode(var_name, expression));
    }

    Node* node = res->register_(bin_op([this]() { return comp_expr(); }, {"and", "or"}, [this]() { return comp_expr(); }));
    if (res->error != nullptr) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'var', 'if', 'for', 'while', 'def', int, float, identifier, '+', '-', '(', '[' or 'not'"));
    return res->success(node);
}

ParseResult* Parser::comp_expr() {
    ParseResult* res = new ParseResult();

    if (current_tok->matches("KEYWORD", "not")) {
        Token* op_tok = current_tok;
        res->register_advancement();
        advance();

        Node* node = res->register_(comp_expr());
        if (res->error != nullptr) return res;
        return res->success(new UnaryOpNode(op_tok, node));
    }

    Node* node = res->register_(bin_op([this]() { return arith_expr(); }, {"EE", "NE", "LT", "GT", "LTE", "GTE"}, [this]() { return arith_expr(); }));
    if (res->error != nullptr) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected int, float, identifier, '+', '-', '(', '[', 'if', 'for', 'while', 'def' or 'not'"));
    return res->success(node);
}

ParseResult* Parser::arith_expr() {
    return bin_op([this]() { return term(); }, {"PLUS", "MINUS"}, [this]() { return term(); });
}

ParseResult* Parser::term() {
    return bin_op([this]() { return factor(); }, {"MUL", "DIV"}, [this]() { return factor(); });
}

ParseResult* Parser::factor() {
    ParseResult* res = new ParseResult();
    Token* tok = current_tok;

    if (in(tok->type, {"PLUS", "MINUS"})) {
        res->register_advancement();
        advance();
        Node* factor_ = res->register_(factor());
        if (res->error != nullptr) return res;
        return res->success(new UnaryOpNode(tok, factor_));
    }

    if (tok->type == "POW") return power();

    return modulus();
}

ParseResult* Parser::power() {
    return bin_op([this]() { return call(); }, {"POW"}, [this]() { return factor(); });
}

ParseResult* Parser::modulus() {
    return bin_op([this]() { return call(); }, {"MOD"}, [this]() { return factor(); });
}

ParseResult* Parser::call() {
    ParseResult* res = new ParseResult();
    Node* atom_ = res->register_(atom());
    if (res->error != nullptr) return res;

    if (current_tok->type == "LPAREN") {
        res->register_advancement();
        advance();
        std::vector<Node*> arg_nodes = {};

        if (current_tok->type == "RPAREN") {
            res->register_advancement();
            advance();
        } else {
            arg_nodes.push_back(res->register_(expr()));
            if (res->error != nullptr) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected ')', 'var', 'if', 'for', 'while', 'def', int, float, identifier, '+', '-', '(', '[' or 'not'"));

            while (current_tok->type == "COMMA") {
                res->register_advancement();
                advance();

                arg_nodes.push_back(res->register_(expr()));
                if (res->error != nullptr) return res;
            }

            if (current_tok->type != "RPAREN") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected ',' or ')'"));
            res->register_advancement();
            advance();
        }

        return res->success(new CallNode(atom_, arg_nodes));
    }

    return res->success(atom_);
}

ParseResult* Parser::atom() {
    ParseResult* res = new ParseResult();
    Token* tok = current_tok;

    if (in(tok->type, {"INT", "FLOAT"})) {
        res->register_advancement();
        advance();
        return res->success(new NumberNode(tok));
    } else if (tok->type == "STRING") {
        res->register_advancement();
        advance();
        return res->success(new StringNode(tok));
    } else if (tok->type == "IDENTIFIER") { //TODO: Add variable recognition. This can only be added after interpreter is completed.
        res->register_advancement();
        advance();
        return res->success(new VarAccessNode(tok));
    } else if (tok->type == "LPAREN") {
        res->register_advancement();
        advance();

        Node* expression = res->register_(expr());
        if (res->error != nullptr) return res;

        if (current_tok->type == "RPAREN") {
            res->register_advancement();
            advance();
            return res->success(expression);
        } else {
            return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected ')'"));
        }
    } else if (tok->type == "LSQUARE") {
        Node* expression = res->register_(list_expr());
        if (res->error != nullptr) return res;
        return res->success(expression);
    } else if (tok->matches("KEYWORD", "if")) {
        Node* expression = res->register_(if_expr());
        if (res->error != nullptr) return res;
        return res->success(expression);
    } else if (tok->matches("KEYWORD", "for")) {
        Node* expression = res->register_(for_expr());
        if (res->error != nullptr) return res;
        return res->success(expression);
    } else if (tok->matches("KEYWORD", "while")) {
        Node* expression = res->register_(while_expr());
        if (res->error != nullptr) return res;
        return res->success(expression);
    } else if (tok->matches("KEYWORD", "def")) {
        Node* expression = res->register_(func_def());
        if (res->error != nullptr) return res;
        return res->success(expression);
    }

    return res->failure(new InvalidSyntaxError(tok->pos_start, tok->pos_end, "Expected int, float, identifier, '+', '-', '(', '[', if', 'for', 'while', 'def'"));
}

ParseResult* Parser::list_expr() {
    ParseResult* res = new ParseResult();
    std::vector<Node*> elements = {};
    Position* pos_start = current_tok->pos_start->copy();

    if (current_tok->type != "LSQUARE") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected '['"));

    res->register_advancement();
    advance();

    if (current_tok->type == "RSQUARE") {
        res->register_advancement();
        advance();
    } else {
        elements.push_back(res->register_(expr()));
        if (res->error != nullptr) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected ']', 'var', 'if', 'for', 'while', 'def', int, float, identifier, '+', '-', '(', '[' or 'not'"));

        while (current_tok->type == "COMMA") {
            res->register_advancement();
            advance();

            elements.push_back(res->register_(expr()));
            if (res->error != nullptr) return res;
        }

        if (current_tok->type != "RSQUARE") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected ',' or ']'"));
        res->register_advancement();
        advance();
    }

    return res->success(new ListNode(elements, pos_start, current_tok->pos_end->copy()));
}

ParseResult* Parser::if_expr() {
    ParseResult* res = new ParseResult();
    Node* all_cases = res->register_(if_expr_cases("if"));
    if (res->error != nullptr) return res;
    std::vector<std::tuple<Node*, Node*, bool>> cases = all_cases->cases;
    std::tuple<Node*, bool> else_case = all_cases->else_case;
    return res->success(new IfNode(cases, else_case));
}

ParseResult* Parser::if_expr_cases(std::string case_keyword) {
    ParseResult* res = new ParseResult();
    std::vector<std::tuple<Node*, Node*, bool>> cases = {};
    std::tuple<Node*, bool> else_case;

    if (!current_tok->matches("KEYWORD", case_keyword)) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected '" + case_keyword + "'"));

    res->register_advancement();
    advance();

    Node* condition = res->register_(expr());
    if (res->error != nullptr) return res;

    if (!current_tok->matches("KEYWORD", "then")) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'then'"));

    res->register_advancement();
    advance();

    if (current_tok->type == "NEWLINE") {
        res->register_advancement();
        advance();

        Node* statements_ = res->register_(statements());
        if (res->error != nullptr) return res;
        cases.emplace_back(condition, statements_, true);

        if (current_tok->matches("KEYWORD", "end")) {
            res->register_advancement();
            advance();
        } else {
            Node* all_cases = res->register_(if_expr_b_or_c());
            if (res->error != nullptr) return res;
            std::vector<std::tuple<Node*, Node*, bool>> new_cases = all_cases->cases;
            std::tuple<Node*, bool> else_case = all_cases->else_case;
            cases.insert(std::end(cases), std::begin(new_cases), std::end(new_cases));
        }
    } else {
        Node* statements_ = res->register_(statements());
        if (res->error != nullptr) return res;
        cases.emplace_back(condition, statements_, false);

        Node* all_cases = res->register_(if_expr_b_or_c());
        if (res->error != nullptr) return res;
        std::vector<std::tuple<Node*, Node*, bool>> new_cases = all_cases->cases;
        std::tuple<Node*, bool> else_case = all_cases->else_case;
        cases.insert(std::end(cases), std::begin(new_cases), std::end(new_cases));
    }

    Node* to_return = new Node();
    to_return->cases = cases;
    to_return->else_case = else_case;
    return res->success(to_return);
}

ParseResult* Parser::if_expr_b_or_c() {
    ParseResult* res = new ParseResult();
    std::vector<std::tuple<Node*, Node*, bool>> cases = {};
    std::tuple<Node*, bool> else_case;

    if (current_tok->matches("KEYWORD", "elif")) {
        Node* all_cases = res->register_(if_expr_b());
        if (res->error != nullptr) return res;
        cases = all_cases->cases;
        std::tuple<Node*, bool> else_case = all_cases->else_case;
    } else {
        Node* else_case = res->register_(if_expr_c());
        if (res->error != nullptr) return res;
    }

    Node* to_return = new Node();
    to_return->cases = cases;
    to_return->else_case = else_case;
    return res->success(to_return);
}

ParseResult* Parser::if_expr_b() {
    return if_expr_cases("elif");
}

ParseResult* Parser::if_expr_c() {
    ParseResult* res = new ParseResult();
    std::tuple<Node*, bool> else_case;

    if (current_tok->matches("KEYWORD", "else")) {
        res->register_advancement();
        advance();

        if (current_tok->type == "NEWLINE") {
            res->register_advancement();
            advance();

            Node* statements_ = res->register_(statements());
            if (res->error != nullptr) return res;
            else_case = std::make_tuple(statements_, true);

            if (current_tok->matches("KEYWORD", "end")) {
                res->register_advancement();
                advance();
            } else {
                return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'end'"));
            }
        } else {
            Node* expression = res->register_(expr());
            if (res->error != nullptr) return res;
            else_case = std::make_tuple(expression, false);
        }
    }

    Node* to_return = new Node();
    to_return->else_case = else_case;
    return res->success(to_return);
}

ParseResult* Parser::for_expr() {
    ParseResult* res = new ParseResult();

    if (!current_tok->matches("KEYWORD", "for")) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'for'"));

    res->register_advancement();
    advance();

    if (current_tok->type != "IDENTIFIER") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected identifier"));

    Token* var_name = current_tok;
    res->register_advancement();
    advance();

    if (current_tok->type != "EQ") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected '='"));

    res->register_advancement();
    advance();

    Node* start_value = res->register_(expr());
    if (res->error != nullptr) return res;

    if (!current_tok->matches("KEYWORD", "to")) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'to'"));

    res->register_advancement();
    advance();

    Node* end_value = res->register_(expr());
    if (res->error != nullptr) return res;

    Node* step_value;
    if (current_tok->matches("KEYWORD", "step")) {
        res->register_advancement();
        advance();

        Node* step_value = res->register_(expr());
        if (res->error != nullptr) return res;
    } else {
        Node* step_value = new Node();
    }

    if (!current_tok->matches("KEYWORD", "then")) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'then'"));

    res->register_advancement();
    advance();

    if (current_tok->type == "NEWLINE") {
        res->register_advancement();
        advance();

        Node* body = res->register_(statements());
        if (res->error != nullptr) return res;

        if (!current_tok->matches("KEYWORD", "end")) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'end'"));

        res->register_advancement();
        advance();

        return res->success(new ForNode(var_name, start_value, end_value, step_value, body, true));
    }

    Node* body = res->register_(statement_());
    if (res->error != nullptr) return res;

    return res->success(new ForNode(var_name, start_value, end_value, step_value, body, false));
}

ParseResult* Parser::while_expr() {
    ParseResult* res = new ParseResult();

    if (!current_tok->matches("KEYWORD", "while")) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'while'"));

    res->register_advancement();
    advance();

    Node* condition = res->register_(expr());
    if (res->error != nullptr) return res;

    if (!current_tok->matches("KEYWORD", "then")) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'then'"));

    res->register_advancement();
    advance();

    if (current_tok->type == "NEWLINE") {
        res->register_advancement();
        advance();

        Node* body = res->register_(statements());
        if (res->error != nullptr) return res;

        if (!current_tok->matches("KEYWORD", "end")) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'end'"));

        res->register_advancement();
        advance();

        return res->success(new WhileNode(condition, body, true));
    }

    Node* body = res->register_(statement_());
    if (res->error != nullptr) return res;

    return res->success(new WhileNode(condition, body, false));
}

ParseResult* Parser::func_def() {
    ParseResult* res = new ParseResult();

    if (!current_tok->matches("KEYWORD", "def")) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'def'"));

    res->register_advancement();
    advance();

    Token* var_name;
    if (current_tok->type == "IDENTIFIER") {
        var_name = current_tok;
        res->register_advancement();
        advance();
        if (current_tok->type != "LPAREN") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected '('"));
    } else {
        var_name = new Token("", "\0", noPos);
        if (current_tok->type != "LPAREN") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected identifier or '('"));
    }

    res->register_advancement();
    advance();
    std::vector<Token*> arg_names = {};

    if (current_tok->type == "IDENTIFIER") {
        arg_names.push_back(current_tok);
        res->register_advancement();
        advance();

        while (current_tok->type == "COMMA") {
            res->register_advancement();
            advance();

            if (current_tok->type == "IDENTIFIER") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected identifier"));

            arg_names.push_back(current_tok);
            res->register_advancement();
            advance();
        }

        if (current_tok->type != "RPAREN") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected ',' or ')'"));
    } else {
        if (current_tok->type != "RPAREN") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected identifier or ')'"));
    }

    res->register_advancement();
    advance();

    if (current_tok->type == "ARROW") {
        res->register_advancement();
        advance();

        Node* body = res->register_(expr());
        if (res->error != nullptr) return res;

        return res->success(new FuncDefNode(var_name, arg_names, body, true));
    }

    if (current_tok->type != "NEWLINE") return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected '->' or newline"));

    res->register_advancement();
    advance();

    Node* body = res->register_(statements());
    if (res->error != nullptr) return res;

    if (!current_tok->matches("KEYWORD", "end")) return res->failure(new InvalidSyntaxError(current_tok->pos_start, current_tok->pos_end, "Expected 'end'"));

    res->register_advancement();
    advance();

    return res->success(new FuncDefNode(var_name, arg_names, body, false));
}

ParseResult* Parser::bin_op(std::function<ParseResult*()> func_a, std::vector<std::string> ops, std::function<ParseResult*()> func_b) {
    ParseResult* res = new ParseResult();
    Node* left = res->register_(func_a());
    if (res->error != nullptr) return res;

    while (in(current_tok->type, ops) || in(current_tok->value, ops)) {
        Token* op_tok = current_tok;
        res->register_advancement();
        advance();
        Node* right = res->register_(func_b());
        if (res->error != nullptr) return res;
        left = new BinOpNode(left, op_tok, right);
    }

    return res->success(left);
}

void Parser::update_tok() {
    current_tok = (token_idx >= 0 && token_idx < tokens.size()) ? tokens[token_idx] : tokens[tokens.size() - 1];
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

Node* ParseResult::register_(ParseResult* res) {
    last_registered_advance_count = res->advance_count;
    advance_count += res->advance_count;
    if (res->error) error = res->error;
    return res->node;
}

Node* ParseResult::try_register(ParseResult* res) {
    if (res->error) {
        to_reverse_count = res->advance_count;
        return new Node();
    }
    return register_(res);
}

ParseResult* ParseResult::success(Node* node_) {
    node = node_;
    return this;
}

ParseResult* ParseResult::failure(Error* error_) {
    if (error == nullptr || (int) last_registered_advance_count == 0) error = error_->copy();
    return this;
}
