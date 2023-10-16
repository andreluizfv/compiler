#include "Utils.h"
#include <map>
#include <variant>
using namespace Utils;

int Utils::searchKeyword(const std::string &word) {
    auto itr = std::lower_bound(reservedKeywords.begin(), reservedKeywords.end(), word);
    if (itr == reservedKeywords.end()) return UNKNOWN;
    return std::distance(reservedKeywords.begin(), itr);
}


int Literals::searchName(std::string& word){
    if (mapObject.find(word) == mapObject.end()){
        mapObject[word] = count;
        count = count + 1;
    }
    return mapObject[word];
}
int Literals::addIntConst(int n){
    constants.emplace_back(n);
    constantsMap[constants.size() - 1] = n;
    return (int) constants.size() - 1;
}
int Literals::addCharConst(char c){
    constants.emplace_back(c);
    constantsMap[constants.size() - 1] = c;
    return (int) constants.size() - 1;
}
int Literals::addStringConst(std::string s){
    constants.emplace_back(s);
    constantsMap[constants.size() - 1] = s;
    return (int) constants.size() - 1;
}


//auto Utils::Literals::mapObject = std::map<std::string, int>();
//auto Utils::Literals::constants = std::vector<std::variant<int,std::string,char>>();
//int Utils::Literals::count = 0;