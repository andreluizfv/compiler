#include <iostream>
#include "Scanner.h"

Scanner::Scanner(std::string source) {
    this->source = std::move(source);

    reservedWords["array"] = ARRAY;
    reservedWords["boolean"] = BOOLEAN;
    reservedWords["break"] = BREAK;
    reservedWords["char"] = CHAR;
    reservedWords["continue"] = CONTINUE;
    reservedWords["do"] = DO;
    reservedWords["else"] = ELSE;
    reservedWords["false"] = FALSE;
    reservedWords["function"] = FUNCTION;
    reservedWords["if"] = IF;
    reservedWords["integer"] = INTEGER;
    reservedWords["of"] = OF;
    reservedWords["string"] = STRING;
    reservedWords["struct"] = STRUCT;
    reservedWords["true"] = TRUE;
    reservedWords["type"] = TYPE;
    reservedWords["var"] = VAR;
    reservedWords["while"] = WHILE;
}

std::vector <Token> Scanner::scanTokens() {
    while(!isEnd()) {
        start = current;
        scanToken();
    }

    return tokens;
}

bool Scanner::isEnd() {
    return current >= source.size();
}

void Scanner::scanToken() {
    char c = parseNextChar();

    switch (c) {
        case '(': addToken(LEFT_PARENTHESIS); break;
        case ')': addToken(RIGHT_PARENTHESIS); break;
        case '[': addToken(LEFT_SQUARE); break;
        case ']': addToken(RIGHT_SQUARE); break;
        case '{': addToken(LEFT_BRACES); break;
        case '}': addToken(RIGHT_BRACES); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '&': addToken(AND); break;
        case '|': addToken(OR); break;

        case '-': addToken(match('-') ? MINUS_MINUS : MINUS); break;
        case '+': addToken(match('+') ? PLUS_PLUS : PLUS); break;
        case '*': addToken(TIMES); break;

        case ';': addToken(SEMI_COLON); break;
        case ':': addToken(COLON); break;


        case '!': addToken(match('=') ? NOT_EQUAL : NOT); break;
        case '=': addToken(match('=') ? EQUAL_EQUAL : EQUALS); break;
        case '<': addToken(match('=') ? LESS_THAN : LESS_OR_EQUAL); break;
        case '>': addToken(match('=') ? GREATER_THAN : GREATER_OR_EQUAL); break;

        case '/':
            if(match('/')) {
                while(peek() != '\n' and !isEnd()) parseNextChar();
            } else addToken(DIVIDE);
            break;

        case ' ':
        case '\r':
        case '\t': break;

        case '\n': line++; break;
        case '"': parseString(); break;
        default:
            if(isDigit(c)) {
                parseNumber();
            } else if(isAlphanumeric(c)) {
                parseIdentifier();
            } else noMatchError();
    }
}

char Scanner::parseNextChar() {
    return source[current++];
}

void Scanner::addToken(TokenType type) {
    std::string txt = source.substr(start, current - start);
    tokens.emplace_back(type, txt, "undefined");
}

void Scanner::addToken(TokenType type, const Object& literal) {
    tokens.emplace_back(type, source.substr(start, current - start), literal);
}

void Scanner::noMatchError() const {
    std::cout << "token did not match any value at line " << line;
}

bool Scanner::match(char expected) {
    if(isEnd() or source[current] != expected) return false;
    current++;
    return true;
}

char Scanner::peek() {
    if(isEnd()) return '\0';
    return source[current];
}

char Scanner::peekNext() {
    if(current + 1 >= source.size()) return '\0';
    return source[current + 1];
}

void Scanner::parseString() {
    while(peek() != '"' and !isEnd()) {
        if(peek() == '\n') line++;
        parseNextChar();
    }

    if(isEnd()) {
        noMatchError();
        return;
    }

    uint len = current - start - 1;
    addToken(STRINGVAL, source.substr(start + 1, len));
    parseNextChar();
}

bool Scanner::isDigit(char c) {
    return c >= '0' and c <= '9';
}

void Scanner::parseNumber() {
    while(isDigit(peek()) and !isEnd()) parseNextChar();

    if(peek() == '.' and isDigit(peekNext())) {
        parseNextChar();
        while (isDigit(peek()) and !isEnd()) parseNextChar();
    }

    addToken(NUMERAL,source.substr(start, current - start));
}

bool Scanner::isAlphanumeric(char c) {
    return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z') or c == '_';
}

void Scanner::parseIdentifier() {
    while(isAlphanumeric(peek()) or isDigit(peek())) parseNextChar();
    std::string text = source.substr(start, current - start);
    auto itr = reservedWords.find(text);
    auto type = itr == reservedWords.end() ? ID : itr->second;
    addToken(type);
}