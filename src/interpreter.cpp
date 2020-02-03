#include <iostream>
#include "mainClasses.hpp"

Number* NumberNull = new Number(0);
Number* NumberFalse = new Number(0);
Number* NumberTrue = new Number(1);
Number* NumberPi = new Number(3.1415926539);
Number* NumberTau = new Number(6.28318530718);
Number* NumberEuler = new Number(2.71828182845);
String_* os = new String_(get_os_name());
String_* os_version = new String_(get_os_version());
String_* cwd = new String_(get_cwd());

RTResult* Interpreter::visit(Node* node, Context* context) {
    std::string node_type = node->type;

    if (node_type == "NumberNode") {
        return visit_NumberNode(node, context);
    } else if (node_type == "StringNode") {
        return visit_StringNode(node, context);
    } else if (node_type == "ListNode") {
        return visit_ListNode(node, context);
    } else if (node_type == "VarAccessNode") {
        return visit_VarAccessNode(node, context);
    } else if (node_type == "VarAssignNode") {
        return visit_VarAssignNode(node, context);
    } else if (node_type == "BinOpNode") {
        return visit_BinOpNode(node, context);
    } else if (node_type == "UnaryOpNode") {
        return visit_UnaryOpNode(node, context);
    } else if (node_type == "IfNode") {
        return visit_IfNode(node, context);
    } else if (node_type == "ForNode") {
        return visit_ForNode(node, context);
    } else if (node_type == "WhileNode") {
        return visit_WhileNode(node, context);
    } else if (node_type == "FuncDefNode") {
        return visit_FuncDefNode(node, context);
    } else if (node_type == "CallNode") {
        return visit_CallNode(node, context);
    } else if (node_type == "ReturnNode") {
        return visit_ReturnNode(node, context);
    } else if (node_type == "ContinueNode") {
        return visit_ContinueNode(node, context);
    } else if (node_type == "BreakNode") {
        return visit_BreakNode(node, context);
    } else {
        throw (node_type);
    }
}

RTResult* Interpreter::visit_NumberNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_StringNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_ListNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_VarAccessNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_VarAssignNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_BinOpNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_UnaryOpNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_IfNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_ForNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_WhileNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_FuncDefNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_CallNode(Node* node, Context* context) {
    return new RTResult();
}

RTResult* Interpreter::visit_ReturnNode(Node* node, Context* context) {
    RTResult* res = new RTResult();
    Value* val = nullptr;

    if (node->to_return != nullptr) {
        val = res->register_(visit(node->to_return, context));
        if (std::get<2>(res->should_return())) return res;
    } else {
        //val = 
    }

    return res->success_return(val);
}

RTResult* Interpreter::visit_ContinueNode(Node* node, Context* context) {
    return (new RTResult())->success_continue();
}

RTResult* Interpreter::visit_BreakNode(Node* node, Context* context) {
    return (new RTResult())->success_break();
}

void RTResult::reset() {
    value = nullptr;
    error = nullptr;
    func_return_value = new Value();
    loop_should_continue = false;
    loop_should_break = false;
}

Value* RTResult::register_(RTResult* res) {
    error = res->error;
    func_return_value = res->func_return_value;
    loop_should_continue = res->loop_should_continue;
    loop_should_break = res->loop_should_break;
    return res->value;
}

RTResult* RTResult::success(Value* val) {
    reset();
    value = val;
    return this;
}

RTResult* RTResult::success_return(Value* val) {
    reset();
    func_return_value = val;
    return this;
}

RTResult* RTResult::success_continue() {
    reset();
    loop_should_continue = true;
    return this;
}

RTResult* RTResult::success_break() {
    reset();
    loop_should_break = true;
    return this;
}

RTResult* RTResult::failure(Error* error_) {
    reset();
    error = error_;
    return this;
}

std::tuple<Error*, Value*, bool> RTResult::should_return() {
    if (error != nullptr) {
        return std::make_tuple(error, nullptr, false);
    } else if (func_return_value != nullptr) {
        return std::make_tuple(error, func_return_value, false);
    } else if (loop_should_continue == true) {
        return std::make_tuple(error, nullptr, true);
    } else {
       return std::make_tuple(error, nullptr, true); 
    }
}

std::string SymbolTable::get(std::string key) {
    std::string value = "\0";
    if (symbols.find(key) != symbols.end()) value = symbols.find(key)->second;
    if (value == "\0" && parent != nullptr) value = parent->get(key);
    return value;
}

void SymbolTable::set(std::string name, std::string value) {
    symbols[name] = value;
}
