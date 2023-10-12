#include <algorithm>
#include "Scope.h"
#include <vector>

using namespace Scope;

Scope::scope_object::scope_object(int nName){ this->nName = nName;}

int Scope::NewBlock(){
    ++nCurrentLevel;
    SymbolTable.push_back(std::vector<scope_object> ());
    return nCurrentLevel;
}

int Scope::EndBlock(){
    return --nCurrentLevel;
}

scope_object Scope::Define(int tokenSecond)
{
    scope_object sObj(tokenSecond);
    sObj.nName = tokenSecond;
    SymbolTable[nCurrentLevel].push_back(sObj);
    return sObj;
}

scope_object Scope::SearchWhenDeclared(int tokenSecond)
{
    scope_object sObj = scope_object(tokenSecond);
    auto itSObj = std::find(SymbolTable[nCurrentLevel].begin(),
                                       SymbolTable[nCurrentLevel].end(),
                                       sObj);
    if (itSObj == SymbolTable[nCurrentLevel].end()){
        SymbolTable[nCurrentLevel].push_back(sObj);
    }
    return sObj;
}

scope_object Scope::FindWhenUsed(int tokenSecond){
    for (int i = nCurrentLevel; i >= 0; --i){
        auto it = std::find(SymbolTable[nCurrentLevel].begin(),
                                       SymbolTable[nCurrentLevel].end(),
                                       scope_object(tokenSecond));
        if(it != SymbolTable[nCurrentLevel].end()) return *it;
    }
    return NULL;
}