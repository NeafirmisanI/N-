#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <tuple>
#include "mainClasses.hpp"

void run(std::string code) {
	Lexer lexer(code);
	std::tuple<std::vector<Token>, Error> res = lexer.lex();

	if (std::get<1>(res).details != "") {
		std::cout << std::get<1>(res).as_string();
		return;
	}


	Parser parser(std::get<0>(res));
	parser.parse();
}

void runRepl() {
	std::cout << "N# Shell v0.0.1\n";
	std::cout << "Press Ctrl+C to exit\n\n";
	
	std::string input;
	
	while (1) {
		std::cout << "N# > ";
		std::getline(std::cin, input);
		if (input == "^C") exit(0);
		if (input.find_first_not_of(' ') == std::string::npos) continue;
		run(input);
		std::cout << "\n";
	}
}

void readFile(std::string filename) {
	std::string text;
	std::string extension = filename.substr(filename.find_last_of("."));
	
	if (extension != ".nsharp") std::cout << "Invalid file extension '" << extension  << "'. Only '.nsharp' files are scanned by interpreter";	
	std::ifstream file(filename);
	
	if (!file.fail()) {
		std::string line;
		
		while (getline(file, line)) {
			text += line.c_str();
		}
		
		file.close();
		text += "\n";
		run(text);
	} else {
		std::cout << "Cannot open file " << filename;
	}
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		runRepl();
	} else {
		std::string fn(argv[1]);
		readFile(fn);
	}
	
	std::cout << "\n";
	getchar();
	return 0;
}
