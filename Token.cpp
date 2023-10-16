#include "Token.h"
#include <iostream>
#include <utility>

Token::Token(TokenType type, std::string lexeme, Object literal) {
    this->type = type;
    this->lexeme = std::move(lexeme);
    this->literal = std::move(literal);
    this->tokenSecond = -1;
    this->const_idx = -1;
}

Token::Token(TokenType type, std::string lexeme, Object literal, int tokenSecond) {
    this->type = type;
    this->lexeme = std::move(lexeme);
    this->literal = std::move(literal);
    this->tokenSecond = -1;
    this->const_idx = tokenSecond;
}

Token::Token(TokenType type, std::string lexeme, Object literal, int tokenSecond, int const_idx) {
    this->type = type;
    this->lexeme = std::move(lexeme);
    this->literal = std::move(literal);
    this->tokenSecond = tokenSecond;
    this->const_idx = const_idx;
}
std::string Token::toString() const {
    bool hasSecond = (type == CHARACTER) ||  (type == NUMERAL) || (type == STRINGVAL)  || (type == ID);
    return hasSecond ?
            ( (std::string) "token: " + lexeme + " aux token: " + std::to_string(tokenSecond) + " const_idx: " + std::to_string(const_idx)) :
            ( (std::string) "token: " + lexeme );
}

