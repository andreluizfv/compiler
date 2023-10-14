#include "Token.h"
#include <iostream>
#include <utility>

Token::Token(TokenType type, std::string lexeme, Object literal) {
    this->type = type;
    this->lexeme = std::move(lexeme);
    this->literal = std::move(literal);
    this->tokenSecond = INT32_MAX;
}

Token::Token(TokenType type, std::string lexeme, Object literal, int tokenSecond) {
    this->type = type;
    this->lexeme = std::move(lexeme);
    this->literal = std::move(literal);
    this->tokenSecond = tokenSecond;
}

std::string Token::toString() const {
    bool hasSecond = (type == CHARACTER) ||  (type == NUMERAL) || (type == STRINGVAL)  || (type == ID);
    return hasSecond ?
            ( (std::string) "token: " + lexeme + " aux token: " + std::to_string(tokenSecond) ) :
            ( (std::string) "token: " + lexeme );
}