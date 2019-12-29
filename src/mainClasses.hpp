#include <functional>
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
        Position* copy();

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
        std::string highlight(std::string text);
    public:
        Position* pos_start;
        Position* pos_end;
        std::string error_name;
        std::string details;
        std::string as_string();
        Error* copy();

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

        Token(std::string type_, std::string val, Position* pos_strt, Position* pos_nd = new Position(-2, -2, -2, "", "")) {
            type = type_;
            value = val;
            pos_start = pos_strt;
            pos_end = pos_nd;
        }
};

class Node {
    public:
        Token* op;
        Token* token;
        Token* var_name;
        Node* body;
        Node* node;
        Node* left;
        Node* value;
        Node* right;
        Node* to_call;
        Node* end_value;
        Node* to_return;
        Node* condition;
        Node* step_value;
        Node* start_value;
        std::vector<std::tuple<Node*, Node*, bool>> cases;
        std::tuple<Node*, bool> else_case;
        std::vector<Token*> arg_names;
        std::vector<Node*> elements;
        std::vector<Node*> args;
        Position* pos_start;
        Position* pos_end;
        bool return_null;
        bool should_return;

        Node() {
            Position* noPos = new Position(-2, -2, -2, "", "");
            pos_start = noPos;
            pos_end = noPos;
        }
};

class NumberNode : public Node {
    public:
        NumberNode(Token* tok) {
            token = tok;
            pos_start = token->pos_start;
            pos_end = token->pos_end;
        }
};

class StringNode : public Node {
    public:
        StringNode(Token* tok) {
            token = tok;
            pos_start = token->pos_start;
            pos_end = token->pos_end;
        }
};

class ListNode : public Node {
    public:
        ListNode(std::vector<Node*> nodes, Position* pos_s, Position* pos_e) {
            elements = nodes;
            pos_start = pos_s;
            pos_end = pos_e;
        }
};

class VarAccessNode : public Node {
    public:
        VarAccessNode(Token* var_name_tok) {
            var_name = var_name_tok;
            pos_start = var_name->pos_start;
            pos_end = var_name->pos_end;
        }
};

class VarAssignNode : public Node {
    public:
        VarAssignNode(Token* var_name_tok, Node* value_node) {
            var_name = var_name_tok;
            value = value_node;
            pos_start = var_name->pos_start;
            pos_end = value->pos_end;
        }
};

class BinOpNode : public Node {
    public:
        BinOpNode(Node* left_node, Token* op_tok, Node* right_node) {
            op = op_tok;
            left = left_node;
            right = right_node;
            pos_start = left->pos_start;
            pos_end = right->pos_end;
        }
};

class UnaryOpNode : public Node {
    public:
        UnaryOpNode(Token* op_tok, Node* node_) {
            op = op_tok;
            node = node_;
            pos_start = op->pos_start;
            pos_end = node->pos_end;
        }
};

class IfNode : public Node {
    public:
        IfNode(std::vector<std::tuple<Node*, Node*, bool>> cases_, std::tuple<Node*, bool> else_case_) {
            cases = cases_;
            else_case = else_case_;
            pos_start = std::get<0>(cases[0])->pos_start;
            pos_end = ((std::get<0>(else_case)->pos_end->inx != -2) ? std::get<0>(else_case) : std::get<0>(cases[cases.size() - 1]))->pos_end;
        }
};

class ForNode : public Node {
    public:
        ForNode(Token* var_name_tok, Node* start_value_node, Node* end_value_node, Node* step_value_node, Node* body_node, bool should_return_null) {
            var_name = var_name_tok;
            start_value = start_value_node;
            end_value = end_value_node;
            step_value = step_value_node;
            body = body_node;
            return_null = should_return_null;
            pos_start = var_name->pos_start;
            pos_end = body->pos_end;
        }
};

class WhileNode : public Node {
    public:
        WhileNode(Node* condition_node, Node* body_node, bool should_return_null) {
            condition = condition_node;
            body = body_node;
            return_null = should_return_null;
            pos_start = condition->pos_start;
            pos_end = body->pos_end;
        }
};

class FuncDefNode : public Node {
    public:
        FuncDefNode(Token* var_name_tok, std::vector<Token*> arg_name_toks, Node* body_node, bool should_auto_return) {
            var_name = var_name_tok;
            arg_names = arg_name_toks;
            body = body_node;
            should_return = should_auto_return;

            if (var_name) {
                pos_start = var_name->pos_start;
            } else if (arg_names.size() > 0) {
                pos_start = arg_names[0]->pos_start;
            } else {
                pos_start = body->pos_start;
            }

            pos_end = body->pos_end;
        }
};

class CallNode : public Node {
    public:
        CallNode(Node* node_to_call, std::vector<Node*> arg_nodes) {
            to_call = node_to_call;
            args = arg_nodes;
            pos_start = to_call->pos_start;
            pos_end = ((args.size() > 0) ? args[args.size() - 1] : to_call)->pos_end;
        }
};

class ReturnNode : public Node {
    public:
        ReturnNode(Node* node_to_return, Position* pos_s, Position* pos_e) {
            to_return = node_to_return;
            pos_start = pos_s;
            pos_end = pos_e;
        }
};

class ContinueNode : public Node {
    public:
        ContinueNode(Position* pos_s, Position* pos_e) {
            pos_start = pos_s;
            pos_end = pos_e;
        }
};

class BreakNode : public Node {
    public:
        BreakNode(Position* pos_s, Position* pos_e) {
            pos_start = pos_s;
            pos_end = pos_e;
        }
};

class ParseResult {
    private:
        Position* noPos;
        int8_t last_registered_advance_count = 0;
        int8_t advance_count = 0;
    public:
        Error* error;
        Node* node;
        int8_t to_reverse_count = 0;
        void register_advancement();
        Node* register_(ParseResult* res);
        Node* try_register(ParseResult* res);
        ParseResult* success(Node* node_);
        ParseResult* failure(Error* error);

        ParseResult() {
            noPos = new Position(-2, -2, -2, "", "");
            error = new Error(noPos, noPos, "", "");
            node = new Node();
        }
};

class Lexer {
    private:
        std::string code;
        Token* noTok;
        Position* pos;
        Position* noPos;
        void advance();
        void skip_comment();
        Token* parse_minus();
        Token* parse_number();
        Token* parse_identifier();
        Token* parse_string();
        Token* parse_equals();
        Token* parse_less_than();
        Token* parse_greater_than();
        Token* make_token(std::string type_);
        std::tuple<Token*, Error> parse_not_equals();
        bool is_digit(char c);
        bool is_identifier(char c);
        char current_char = '\0';
    public:
        std::tuple<std::vector<Token*>, Error> lex();

        Lexer(std::string text) {
            noPos = new Position(-2, -2, -2, "", "");
            noTok = new Token("\0", "\0", noPos, noPos);
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
        std::vector<Token*> tokens;
        void update_tok();
        Token reverse(int amount);
        Token advance();
        ParseResult* call();
        ParseResult* atom();
        ParseResult* expr();
        ParseResult* term();
        ParseResult* power();
        ParseResult* factor();
        ParseResult* modulus();
        ParseResult* if_expr();
        ParseResult* for_expr();
        ParseResult* func_def();
        ParseResult* comp_expr();
        ParseResult* list_expr();
        ParseResult* if_expr_b();
        ParseResult* if_expr_c();
        ParseResult* arith_expr();
        ParseResult* while_expr();
        ParseResult* statements();
        ParseResult* statement_();
        ParseResult* if_expr_b_or_c();
        ParseResult* if_expr_cases(std::string case_keyword);
        ParseResult* bin_op(std::function<ParseResult*()> func_a, std::vector<std::string> ops, std::function<ParseResult*()> func_b);
    public:
        ParseResult* parse();

        Parser(std::vector<Token*> toks) {
            noPos = new Position(-2, -2, -2, "", "");
            tokens = toks;
            advance();
        }
};

inline bool in(char c, std::string str) {
    return str.find(c) != std::string::npos;
}

inline bool in(std::string str, std::vector<std::string> array) {
    return find(array.begin(), array.end(), str) != array.end();
}
