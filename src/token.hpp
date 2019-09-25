#include <iostream>

using namespace std;

class Token {
    public:
        string value;
        string type;
        int pos_start;
        int pos_end;
        bool matches(string type_, string val);
        string repr();

        Token(string type_, string val, int pos_strt, int pos_nd) {
            type = type_;
            value = val;

            if (pos_strt != -2) {
                pos_start = pos_strt;
                pos_end = pos_start;
            }

            if (pos_nd != -2) {
                pos_end = pos_nd;
            }
        }
};
