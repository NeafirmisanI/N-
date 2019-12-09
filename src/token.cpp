#include <iostream>
#include "mainClasses.hpp"

bool Token::matches(std::string type_, std::string val) {
    return (type == type_) && (value == val);
}

std::string Token::repr() {
    if (value != "\0") return type + ":" + value;
    return type;
}
