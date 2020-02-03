#include <iostream>
#include "mainClasses.hpp"

Error* Error::copy() {
    return new Error(pos_start, pos_end, error_name, details);
}

std::string Error::as_string() {
    std::string result = error_name + ": " + details + "\n\n";
    result += "File " + pos_start->fn + ", line " + std::to_string(pos_start->ln + 1);
    result += "\n\n" + highlight(pos_start->ftext);
    return result;
}

std::string RTError::as_string() {
    std::string result = generate_traceback();
    result += error_name + ": " + details;
    result += "\n\n" + highlight(pos_start->ftext);
    return result;
}

std::string RTError::generate_traceback() {
    std::string result = "";
    Position* pos = pos_start;
    Context* ctx = context;

    while (ctx != nullptr) {
        result = "  File " + pos->fn + ", line " + std::to_string((int)(pos->ln) + 1) + ", in " + ctx->display_name + "\n" + result;
        pos = ctx->parent_entry_pos;
        ctx = ctx->parent;
    }

    return "Traceback (most recent call last):\n" + result;
}

std::string Error::highlight(std::string text) {
    std::string result = "";
    int8_t idx_start = std::max((int)(text.rfind("\n", 0, pos_start->inx)), 0);
    int8_t idx_end = (int)(text.find("\n", idx_start + 1));
    if (idx_end < 0) idx_end = text.length();
    int8_t linec = pos_end->ln - pos_start->ln + 1;

    for (int8_t i = 0; i < linec; i++) {
        std::string line = text.substr(idx_start, idx_end);
        int8_t col_start = (i == 0) ? pos_start->col : 0;
        int8_t col_end = (i == linec - 1) ? pos_end->col : line.length() - 1;
        result += line + '\n';
        result += std::string(col_start, ' ') + std::string(col_end - col_start, '^');
        idx_start = idx_end;
        idx_end = (int8_t)(text.find("\n", idx_start + 1));
        if (idx_end < 0) idx_end = text.length();
    }

    return result;
}
