//
// Created by pbbdasilva on 8/23/23.
//
#include "Utils.h"
using namespace Utils;

int Utils::searchKeyword(const std::string &word) {
    auto itr = std::lower_bound(reservedKeywords.begin(), reservedKeywords.end(), word);
    if (itr == reservedKeywords.end()) return UNKNOWN;
    return std::distance(reservedKeywords.begin(), itr);
}