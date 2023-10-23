//
// Created by pbbdasilva on 10/14/23.
//

#ifndef COMPILER_SYNTACTIC_H
#define COMPILER_SYNTACTIC_H
#include <vector>
#include "Token.h"

class Syntactic {
public:
    Syntactic(const std::vector<Token>& tokens, FILE* codeFile);
    int nextToken(std::vector<Token>::const_iterator& iterator);
    void error(const Token &token, int idx);

    std::vector<Token> tokens;
};

#endif //COMPILER_SYNTACTIC_H
