#include "Token.h"

#include <utility>

Token::Token(TokenType type, std::string lexeme, Object literal) {
    this->type = type;
    this->lexeme = std::move(lexeme);
    this->literal = std::move(literal);
}

std::string Token::toString() const {
    return lexeme;
}