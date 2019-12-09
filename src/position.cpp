#include <iostream>
#include "mainClasses.hpp"

void Position::advance() {
    ++inx;
    ++col;

    if (ftext[inx] == '\n') {
        ++ln;
        col = 0;
    }
}
