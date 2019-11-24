#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include "position.hpp"

using namespace std;

class Error {
    public:
        Position* pos_start;
        Position* pos_end;
        string error_name;
        string details;
        string as_string();
        string highlight(string text, Position* pos_start, Position* pos_end);

        Error(Position* pos_s, Position* pos_e, string error_nam, string detail) {
            pos_start = pos_s;
            pos_end = pos_e;
            error_name = error_nam;
            details = detail;
        }
};
