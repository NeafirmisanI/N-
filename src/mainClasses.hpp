#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd
#elif _WIN64
#include <windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <sys/utsname.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include <functional>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>
#include <tuple>
#include <any>
#include <map>

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

class SymbolTable {
    public:
        std::map<std::string, std::string> symbols;
        SymbolTable* parent;
        std::string get(std::string key);
        void set(std::string name, std::string value);
        void remove();

        SymbolTable(SymbolTable* parent_ = nullptr) {
            parent = parent_;
        }
};

class Context {
    public:
        std::string display_name;
        Context* parent;
        Position* parent_entry_pos;
        SymbolTable* symbol_table;

        Context(std::string display_name_, Context* parent_ = nullptr, Position* parent_entry_pos_ = new Position(-2, -2, -2, "", "")) {
            display_name = display_name_;
            parent = parent_;
            parent_entry_pos = parent_entry_pos_;
        }
};

class Error {
    public:
        Position* pos_start;
        Position* pos_end;
        std::string error_name;
        std::string details;
        std::string as_string();
        std::string highlight(std::string text);
        Error* copy();

        Error(Position* pos_s, Position* pos_e, std::string error_nam, std::string detail) {
            pos_start = pos_s;
            pos_end = pos_e;
            error_name = error_nam;
            details = detail;
        }
};

class IllegalCharError : public Error {
    public:
        IllegalCharError(Position* pos_start, Position* pos_end, std::string details) : Error(pos_start, pos_end, "Illegal Character", details) { }
};

class ExpectedCharError : public Error {
    public:
        ExpectedCharError(Position* pos_start, Position* pos_end, std::string details) : Error(pos_start, pos_end, "Expected Character", details) { }
};

class InvalidSyntaxError : public Error {
    public:
        InvalidSyntaxError(Position* pos_start, Position* pos_end, std::string details) : Error(pos_start, pos_end, "Invalid Syntax", details) { }
};

class RTError : public Error {
    public:
        Context* context;
        std::string as_string();
        std::string generate_traceback();

        RTError(Position* pos_start, Position* pos_end, std::string details, Context* context_) : Error(pos_start, pos_end, "Runtime Error", details) {
            context = context_;
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

        Token(std::string type_, std::string val, Position* pos_strt, Position* pos_nd = nullptr) {
            type = type_;
            value = val;
            pos_start = pos_strt;
            pos_end = pos_nd;
        }
};

class Node {
    public:
        Token* op = nullptr;
        Token* token = nullptr;
        Token* var_name = nullptr;
        Node* body = nullptr;
        Node* node = nullptr;
        Node* left = nullptr;
        Node* value = nullptr;
        Node* right = nullptr;
        Node* to_call = nullptr;
        Node* end_value = nullptr;
        Node* to_return = nullptr;
        Node* condition = nullptr;
        Node* step_value = nullptr;
        Node* start_value = nullptr;
        std::string type;
        std::vector<std::tuple<Node*, Node*, bool>> cases;
        std::tuple<Node*, bool> else_case;
        std::vector<Token*> arg_names;
        std::vector<Node*> elements;
        std::vector<Node*> args;
        Position* pos_start;
        Position* pos_end;
        Position* noPos = nullptr;
        bool return_null;
        bool should_return;

        Node() {
            pos_start = noPos;
            pos_end = noPos;
        }
};

class NumberNode : public Node {
    public:
        NumberNode(Token* tok) {
            type = "NumberNode";
            token = tok;
            pos_start = token->pos_start;
            pos_end = token->pos_end;
        }
};

class StringNode : public Node {
    public:
        StringNode(Token* tok) {
            type = "StringNode";
            token = tok;
            pos_start = token->pos_start;
            pos_end = token->pos_end;
        }
};

class ListNode : public Node {
    public:
        ListNode(std::vector<Node*> nodes, Position* pos_s, Position* pos_e) {
            type = "ListNode";
            elements = nodes;
            pos_start = pos_s;
            pos_end = pos_e;
        }
};

class VarAccessNode : public Node {
    public:
        VarAccessNode(Token* var_name_tok) {
            type = "VarAccessNode";
            var_name = var_name_tok;
            pos_start = var_name->pos_start;
            pos_end = var_name->pos_end;
        }
};

class VarAssignNode : public Node {
    public:
        VarAssignNode(Token* var_name_tok, Node* value_node) {
            type = "VarAssignNode";
            var_name = var_name_tok;
            value = value_node;
            pos_start = var_name->pos_start;
            pos_end = value->pos_end;
        }
};

class BinOpNode : public Node {
    public:
        BinOpNode(Node* left_node, Token* op_tok, Node* right_node) {
            type = "BinOpNode";
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
            type = "UnaryOpNode";
            op = op_tok;
            node = node_;
            pos_start = op->pos_start;
            pos_end = node->pos_end;
        }
};

class IfNode : public Node {
    public:
        IfNode(std::vector<std::tuple<Node*, Node*, bool>> cases_, std::tuple<Node*, bool> else_case_) {
            type = "IfNode";
            cases = cases_;
            else_case = else_case_;
            pos_start = std::get<0>(cases[0])->pos_start;
            pos_end = ((std::get<0>(else_case)->pos_end->inx != -2) ? std::get<0>(else_case) : std::get<0>(cases[cases.size() - 1]))->pos_end;
        }
};

class ForNode : public Node {
    public:
        ForNode(Token* var_name_tok, Node* start_value_node, Node* end_value_node, Node* step_value_node, Node* body_node, bool should_return_null) {
            type = "ForNode";
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
            type = "WhileNode";
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
            type = "FuncDefNode";
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
            type = "CallNode";
            to_call = node_to_call;
            args = arg_nodes;
            pos_start = to_call->pos_start;
            pos_end = ((args.size() > 0) ? args[args.size() - 1] : to_call)->pos_end;
        }
};

class ReturnNode : public Node {
    public:
        ReturnNode(Node* node_to_return, Position* pos_s, Position* pos_e) {
            type = "ReturnNode";
            to_return = node_to_return;
            pos_start = pos_s;
            pos_end = pos_e;
        }
};

class ContinueNode : public Node {
    public:
        ContinueNode(Position* pos_s, Position* pos_e) {
            type = "ContinueNode";
            pos_start = pos_s;
            pos_end = pos_e;
        }
};

class BreakNode : public Node {
    public:
        BreakNode(Position* pos_s, Position* pos_e) {
            type = "BreakNode";
            pos_start = pos_s;
            pos_end = pos_e;
        }
};

class ParseResult {
    private:
        Position* noPos = nullptr;
        int8_t last_registered_advance_count = 0;
        int8_t advance_count = 0;
    public:
        Error* error = nullptr;
        Node* node = nullptr;
        int8_t to_reverse_count = 0;
        void register_advancement();
        Node* register_(ParseResult* res);
        Node* try_register(ParseResult* res);
        ParseResult* success(Node* node_);
        ParseResult* failure(Error* error);

        ParseResult() { }
};

class Value;

class RTResult {
    private:
        void reset();
        Position* noPos = nullptr;
    public:
        Value* value;
        Error* error;
        Value* func_return_value;
        bool loop_should_continue;
        bool loop_should_break;
        Value* register_(RTResult* res);
        RTResult* success(Value* val);
        RTResult* failure(Error* error_);
        RTResult* success_break();
        RTResult* success_continue();
        RTResult* success_return(Value* val);
        std::tuple<Error*, Value*, bool> should_return();

        RTResult() {
            reset();
        }
};

class Value {
    public:
        int8_t int_value;
        std::string str_value;
        Context* context;
        Position* pos_start;
        Position* pos_end;
        Value* set_pos(Position* pos_s = nullptr, Position* pos_e = nullptr);
        Value* set_context(Context* context_ = nullptr);
        virtual std::tuple<Value*, Error*> added_to(Value* other);
        virtual std::tuple<Value*, Error*> subbed_by(Value* other);
        virtual std::tuple<Value*, Error*> multed_by(Value* other);
        virtual std::tuple<Value*, Error*> dived_by(Value* other);
        virtual std::tuple<Value*, Error*> powed_by(Value* other);
        virtual std::tuple<Value*, Error*> modded_by(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_eq(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_ne(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_lt(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_gt(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_lte(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_gte(Value* other);
        virtual std::tuple<Value*, Error*> anded_by(Value* other);
        virtual std::tuple<Value*, Error*> ored_by(Value* other);
        virtual std::tuple<Value*, Error*> notted();
        virtual std::tuple<Value*, Error*> round();
        virtual std::tuple<Value*, Error*> abs();
        virtual std::tuple<Value*, Error*> floor();
        virtual std::tuple<Value*, Error*> ceil();
        virtual std::tuple<Value*, Error*> sin();
        virtual std::tuple<Value*, Error*> cos();
        virtual std::tuple<Value*, Error*> tan();
        RTResult* execute(std::vector<Value*> args);
        Value* copy();
        bool is_true();
        Error* illegal_operation(Value* other = nullptr);

        Value() {
            set_pos();
            set_context();
        }
};

class Number : public Value {
    public:
        int8_t value;
        virtual std::tuple<Value*, Error*> added_to(Value* other);
        virtual std::tuple<Value*, Error*> subbed_by(Value* other);
        virtual std::tuple<Value*, Error*> multed_by(Value* other);
        virtual std::tuple<Value*, Error*> dived_by(Value* other);
        virtual std::tuple<Value*, Error*> powed_by(Value* other);
        virtual std::tuple<Value*, Error*> modded_by(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_eq(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_ne(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_lt(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_gt(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_lte(Value* other);
        virtual std::tuple<Value*, Error*> get_comparison_gte(Value* other);
        virtual std::tuple<Value*, Error*> anded_by(Value* other);
        virtual std::tuple<Value*, Error*> ored_by(Value* other);
        virtual std::tuple<Value*, Error*> notted();
        virtual std::tuple<Value*, Error*> round();
        virtual std::tuple<Value*, Error*> abs();
        virtual std::tuple<Value*, Error*> floor();
        virtual std::tuple<Value*, Error*> ceil();
        virtual std::tuple<Value*, Error*> sin();
        virtual std::tuple<Value*, Error*> cos();
        virtual std::tuple<Value*, Error*> tan();
        Value* copy();
        bool is_true();
        std::string repr();
        
        Number(int8_t val) : Value() {
            int_value = val;
        }
};

class String_ : public Value {
    public:
        std::string str_value;
        virtual std::tuple<Value*, Error*> added_to(Value* other);
        virtual std::tuple<Value*, Error*> multed_by(Value* other);
        Value* copy();
        bool is_true();
        std::string repr();

        String_(std::string val) : Value() {
            str_value = val;
        }
};

class Lexer {
    private:
        std::string code;
        Token* noTok = nullptr;
        Position* pos;
        Position* noPos = nullptr;
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
        std::tuple<Token*, Error*> parse_not_equals();
        bool is_digit(char c);
        bool is_identifier(char c);
        char current_char = '\0';
    public:
        std::tuple<std::vector<Token*>, Error*> lex();

        Lexer(std::string text) {
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
        Position* noPos = nullptr;
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
            tokens = toks;
            advance();
        }
};

class Interpreter {
    private:
        // Stuff
    public:
        RTResult* visit(Node* node, Context* context);
        RTResult* visit_NumberNode(Node* node, Context* context);
        RTResult* visit_StringNode(Node* node, Context* context);
        RTResult* visit_ListNode(Node* node, Context* context);
        RTResult* visit_VarAccessNode(Node* node, Context* context);
        RTResult* visit_VarAssignNode(Node* node, Context* context);
        RTResult* visit_BinOpNode(Node* node, Context* context);
        RTResult* visit_UnaryOpNode(Node* node, Context* context);
        RTResult* visit_IfNode(Node* node, Context* context);
        RTResult* visit_ForNode(Node* node, Context* context);
        RTResult* visit_WhileNode(Node* node, Context* context);
        RTResult* visit_FuncDefNode(Node* node, Context* context);
        RTResult* visit_CallNode(Node* node, Context* context);
        RTResult* visit_ReturnNode(Node* node, Context* context);
        RTResult* visit_ContinueNode(Node* node, Context* context);
        RTResult* visit_BreakNode(Node* node, Context* context);

        Interpreter() {

        }
};

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

inline bool in(char c, std::string str) {
    return str.find(c) != std::string::npos;
}

inline bool in(std::string str, std::vector<std::string> array) {
    return find(array.begin(), array.end(), str) != array.end();
}

inline std::string get_os_name() {
    #ifdef _WIN32
    return "Windows";
    #elif _WIN64
    return "Windows";
    #elif __APPLE__ || __MACH__
    return "Mac OS";
    #elif __linux__
    return "Linux";
    #elif __unix || __unix__
    return "Unix";
    #else
    return "Unknown";
    #endif
}

inline std::string get_windows_version() {
    OSVERSIONINFOEX info;
    ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    GetVersionEx((LPOSVERSIONINFO) &info);

    if (info.dwMajorVersion == 6) {
        if (info.dwMinorVersion == 0) {
            if (info.wProductType == VER_NT_WORKSTATION) {
                return "Vista";
            } else {
                return "Server 2008";
            }
        } else if (info.dwMinorVersion == 1) {
            if (info.wProductType == VER_NT_WORKSTATION) {
                return "7";
            } else {
                return "Server 2008 R2";
            }
        } else if (info.dwMinorVersion == 2) {
            if (info.wProductType == VER_NT_WORKSTATION) {
                return "8";
            } else {
                return "Server 2012";
            }
        } else if (info.dwMinorVersion == 3) {
            if (info.wProductType == VER_NT_WORKSTATION) {
                return "8.1 or 10";
            } else {
                return "Server 2012 R2";
            }
        }
    } else if (info.dwMajorVersion == 5) {
        if (info.dwMinorVersion == 2) {
            if (info.wProductType == VER_NT_WORKSTATION) {
                return "XP";
            } else if (GetSystemMetrics(SM_SERVERR2) == 0) {
                return "Server 2003";
            } else if (GetSystemMetrics(SM_SERVERR2) != 0) {
                return "Server 2003 R2";
            } else {
                return "Home Server";
            }
        } else if (info.dwMinorVersion == 3) {
            if (info.wProductType == VER_NT_WORKSTATION) {
                return "8.1 or 10";
            } else {
                return "Server 2012 R2";
            }
        }
    }

    return "";
}

inline std::string get_os_version() {
    #ifdef _WIN32
    return get_windows_version();
    #elif _WIN64
    return get_windows_version();
    #elif __APPLE__ || __MACH__ || __unix || __unix__ || __linux__
    struct utsname uts;
    uname(&uts);
    return uts.release;
    #endif

    return "";
}

inline std::string get_cwd() {
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}
