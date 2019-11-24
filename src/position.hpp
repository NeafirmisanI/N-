#include <iostream>

using namespace std;

class Position {
    public:
        int inx;
        int ln;
        int col;
        string fn;
        string ftext;
        void advance();

        Position(int index, int line, int column, string filename, string filetext) {
            inx = index;
            ln = line;
            col = column;
            fn = filename;
            ftext = filetext;
        }
};
