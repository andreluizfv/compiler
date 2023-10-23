#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Scanner.h"
#include "Token.h"
#include "Syntactic.h"

void run(std::string source, std::ofstream codeFile) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    for(auto token : tokens) {
        std::cout << token.toString() << std::endl;
    }
    Syntactic syntactic(tokens, codeFile);
}

void runFile(std::string filePath, std::ofstream codeFile) {
    std::ifstream file(filePath);
    std::stringstream buffer;

    if(file.is_open()) {
        buffer << file.rdbuf();
        file.close();
        run(buffer.str(), codeFile);
    }
}

void runPrompt(std::ofstream codeFile) {
    for (;;) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) break;
        run(line, codeFile);
    }
}

int main(int argc, char* argv[]) {
    std::ofstream codeFile("codeFile.txt");
    if (argc > 2) {
        std::cout << "Too many cli args";
        exit(64);
    } else if(argc == 2){
        runFile(argv[1], codeFile);
    } else {
        runPrompt(codeFile);
    }
}