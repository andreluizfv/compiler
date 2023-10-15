#pragma once

#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H
#include<map>
#include <string>
#include <variant>
#include <vector>
#include <algorithm>
#include <string>
#include "TokenType.h"
#include "ParserTables.h"
namespace Utils {
    static std::vector<std::string> reservedKeywords = {
            "ARRAY", "BOOLEAN", "BREAK", "CHAR", "CONTINUE", "DO", "ELSE", "FALSE", "FUNCTION", "IF",
            "INTEGER", "OF", "RETURN","STRING", "STRUCT", "TRUE", "TYPE", "VAR", "WHILE"};

    int searchKeyword(const std::string &word);

    class Literals {
    private:
        inline static std::map<std::string, int> mapObject;
        inline static std::vector<std::variant<int,std::string,char>> constants;
        inline static int count;
    public:
        static int searchName(std::string &word);
        static int addCharConst(char c);
        static int addStringConst(std::string s);
        static int addIntConst(int n);
    };

    class RedeclaredException : public std::exception{
    public:
        std::string what(){
            return "Variable redeclared";
        }
    };
    class NotDeclaredException : public std::exception{
    public:
        std::string what(){
            return "Variable redeclared";
        }
    };
}

#endif //COMPILER_UTILS_H
