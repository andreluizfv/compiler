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
    int const_idx;
    int tokenSecond;
    Token(TokenType type, std::string lexeme, Object literal);
    Token(TokenType type, std::string lexeme, Object literal, int const_idx);
    Token(TokenType type, std::string lexeme, Object literal, int tokenSecond, int const_idx);
    std::string toString() const;
};


#endif //COMPILER_TOKEN_H