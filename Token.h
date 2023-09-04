#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include <string>
#include <variant>
#include "TokenType.h"

typedef std::variant<std::monostate, std::string, double, char> Object;

class Token {
public:
    TokenType type;
    std::string lexeme;
    Object literal;
    int tokenSecond;

    Token(TokenType type, std::string lexeme, Object literal);
    Token(TokenType type, std::string lexeme, Object literal, int tokenSecond);
    std::string toString() const;
};


#endif //COMPILER_TOKEN_H