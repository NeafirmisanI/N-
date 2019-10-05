#include <iostream>
#include "error.hpp"

using namespace std;

string Error::as_string() {
    string result = error_name + ": " + details + "\n";
    result += "File <file>, line <line>";
    result += "\n\n" + highlight("text", pos_start, pos_end);
    return result;
}

string Error::highlight(string text, int pos_start, int pos_end) {
    return "test";
}