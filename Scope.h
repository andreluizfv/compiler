#pragma once
#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H
#include <vector>
using namespace std;
namespace Scope
{
    class scope_object{
    public:
        int nName;
        scope_object(int nName);
        bool operator==(const scope_object &obj) const {
            return obj.nName == nName;
        };
    };

    static vector<vector<scope_object>> SymbolTable;
    static int nCurrentLevel = -1;

    int NewBlock();
    int EndBlock();
    scope_object Define(int tokenSecond);
    std::vector<scope_object>::iterator SearchWhenDeclared(int tokenSecond);
    std::vector<scope_object>::iterator FindWhenUsed(int tokenSecond);
    
}

#endif // COMPILER_TOKEN_H