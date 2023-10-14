#pragma once
#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H
enum TokenType {
    //reserved keywords
    ARRAY, BOOLEAN, BREAK, CHAR, CONTINUE, DO, ELSE, FALSE, FUNCTION, IF, 
    INTEGER, OF, RETURN, STRING, STRUCT, TRUE, TYPE, VAR, WHILE,
    // symbols
    COLON, SEMI_COLON, COMMA, EQUALS, LEFT_SQUARE, RIGHT_SQUARE, 
    LEFT_BRACES, RIGHT_BRACES, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, AND, 
    OR, LESS_THAN, GREATER_THAN, LESS_OR_EQUAL, GREATER_OR_EQUAL, 
    NOT_EQUAL, EQUAL_EQUAL, PLUS, PLUS_PLUS, MINUS, MINUS_MINUS, TIMES, 
    DIVIDE, DOT, NOT,
    // regular tokens
    CHARACTER, NUMERAL, STRINGVAL, ID,
    // no one else
    UNKNOWN
};

//static const std::string toString(TokenType t) {
//    switch (t) {
//        case
//    }
//}

#endif // TOKEN_TYPE_H