#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "lexer.hpp"

using namespace std;

void run(string code) {
	Lexer lexer(code);

	lexer.lex();
}

void runRepl() {
	cout << "N# Shell v0.0.1\n";
	cout << "Press Ctrl+C to exit\n\n";
	
	string input;
	
	while (1) {
		cout << "N# > ";
		getline(cin, input);
		input += "\n";
		run(input);
		cout << "\n";
	}
}

void readFile(char* filename) {
	string text;
	ifstream file(filename);
	
	if (!file.fail()) {
		string line;
		
		while (getline(file, line)) {
			text += line.c_str();
		}
		
		file.close();
		text += "\n";
		run(text);
	} else {
		cout << "Cannot open file " << filename;
	}
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		runRepl();
	} else {
		readFile(argv[1]);
	}
	
	cout << "\n";
	getchar();
	return 0;
}
