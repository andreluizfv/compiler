#ifndef COMPILER_SEMANTICS_H
#define COMPILER_SEMANTICS_H

#include "Syntactic.h"
#include "ParserTables.h"
#include "Utils.h"
#include "Scope.h"
#include <vector>

namespace Semantics{

    static std::vector<Scope::t_attrib> stackSem;
    
    void semantics( RuleSign ruleNo, int tokenSecond);
    
    void pushSem (Scope::t_attrib &attrib);
    void popSem(int popsNumber);
    Scope::t_attrib topSem(int offsetFromTop);
}
#endif // COMPILER_SEMANTICS_H