//
// Created by pbbdasilva on 8/23/23.
//

#pragma once

#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H

#include <string>
#include <vector>
#include <algorithm>

#include "t_token.h"
namespace Utils {
    static std::vector<std::string> reservedKeywords = {
            "ARRAY", "BOOLEAN", "BREAK", "CHAR", "CONTINUE", "DO", "ELSE", "FALSE", "FUNCTION", "IF",
            "INTEGER", "OF", "STRING", "STRUCT", "TRUE", "TYPE", "VAR", "WHILE"};
    int searchKeyword(const std::string& word);
}

#endif //COMPILER_UTILS_H
