#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

void run(string code) {
	cout << code;
}

void runRepl() {
	cout << "N# Shell v0.0.1\n";
	cout << "Press Ctrl+C to exit\n\n";
	
	string input;
	
	while (1) {
		cout << "N# > ";
		cin >> input;
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
