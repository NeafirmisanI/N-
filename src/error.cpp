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

std::string Error::highlight(std::string text) {
    std::string result = "";
    int8_t idx_start = std::max(static_cast<int>(text.rfind("\n", 0, pos_start->inx)), 0);
    int8_t idx_end = static_cast<int>(text.find("\n", idx_start + 1));
    if (idx_end < 0) idx_end = text.length();
    int8_t linec = pos_end->ln - pos_start->ln + 1;

    for (int8_t i = 0; i < linec; i++) {
        std::string line = text.substr(idx_start, idx_end);
        int8_t col_start = (i == 0) ? pos_start->col : 0;
        int8_t col_end = (i == linec - 1) ? pos_end->col : line.length() - 1;
        result += line + '\n';
        result += std::string(col_start, ' ') + std::string(col_end - col_start, '^');
        idx_start = idx_end;
        idx_end = static_cast<int8_t>(text.find("\n", idx_start + 1));
        if (idx_end < 0) idx_end = text.length();
    }

    return result;
}
