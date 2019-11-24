#include <iostream>

#include "error.hpp"

using namespace std;

string Error::as_string() {
    string result = error_name + ": " + details + "\n";
    result += "File " + pos_start->fn + ", line " + to_string(pos_start->ln + 1);
    result += "\n\n" + highlight(pos_start->ftext, pos_start, pos_end);
    return result;
}

string Error::highlight(string text, Position* pos_start, Position* pos_end) {
    string result = "";
    int idx_start = max(static_cast<int>(text.rfind("\n", 0, pos_start->inx)), 0);
    int idx_end = static_cast<int>(text.find("\n", idx_start + 1));
    if (idx_end < 0) idx_end = text.length();
    int linec = pos_end->ln - pos_start->ln + 1;

    for (int i = 0; i < linec; i++) {
        string line = text.substr(idx_start, idx_end);
        int col_start = (i == 0) ? pos_start->col : 0;
        int col_end = (i == linec - 1) ? pos_end->col : line.length() - 1;
        result += line + '\n';
        result += string(col_start, ' ') + string(col_end - col_start, '^');
        idx_start = idx_end;
        idx_end = static_cast<int>(text.find("\n", idx_start + 1));
        if (idx_end < 0) idx_end = text.length();
    }

    return result;
}
