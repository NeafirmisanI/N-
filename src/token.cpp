#include <iostream>
#include "token.hpp"

using namespace std;

bool Token::matches(string type_, string val) {
    return (type == type_) && (value == val);
}

string Token::repr() {
    if (value != "\0") return type + ":" + value;
    return type;
}