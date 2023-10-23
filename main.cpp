#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Scanner.h"
#include "Token.h"
#include "Syntactic.h"
#include "stdio.h"

void run(std::string source, FILE* codeFile) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    for(auto token : tokens) {
        std::cout << token.toString() << std::endl;
    }
    Syntactic syntactic(tokens, codeFile);
}

void runFile(std::string filePath, FILE* codeFile) {
    std::ifstream file(filePath);
    std::stringstream buffer;

    if(file.is_open()) {
        buffer << file.rdbuf();
        file.close();
        run(buffer.str(), codeFile);
    }
}

void runPrompt(FILE* codeFile) {
    for (;;) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) break;
        run(line, codeFile);
    }
}

int main(int argc, char* argv[]) {
    FILE* codeFile = fopen("codeFile.txt", "w");
    if (codeFile == nullptr)
    {
        printf("Error! Could not open file\n");
        exit(-1); // must include stdlib.h
    }
    if (argc > 2) {
        std::cout << "Too many cli args";
        exit(64);
    } else if(argc == 2){
        runFile(argv[1], codeFile);
    } else {
        runPrompt(codeFile);
    }
    fclose(codeFile);
}