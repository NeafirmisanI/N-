#include <iostream>
#include "position.hpp"

using namespace std;

void Position::advance() {
    ++inx;
    ++col;

    if (ftext[inx] == '\n') {
        ++ln;
        col = 0;
    }
}
