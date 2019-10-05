#include <iostream>

using namespace std;

class Error {
    public:
        int pos_start;
        int pos_end;
        string error_name;
        string details;
        string as_string();
        string highlight(string text, int pos_start, int pos_end);

        Error(int pos_s, int pos_e, string error_nam, string detail) {
            pos_start = pos_s;
            pos_end = pos_e;
            error_name = error_nam;
            details = detail;
        }
};