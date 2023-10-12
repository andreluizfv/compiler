#pragma once
#ifndef COMPILER_SCOPE_H
#define COMPILER_SCOPE_H
#include <vector>
using namespace std;
namespace Scope
{
    class scope_object{
    public:
        int nName;
        scope_object(int nName);
    };

    vector<vector<scope_object>> SymbolTable;
    int nCurrentLevel = -1;

    int NewBlock();
    int EndBlock();
    scope_object Define(int tokenSecond);
    scope_object SearchWhenDeclared(int tokenSecond);
    scope_object FindWhenUsed(int tokenSecond);
}

#endif // COMPILER_SCOPE_H