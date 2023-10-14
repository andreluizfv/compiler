#ifndef COMPILER_SEMANTICS_H
#define COMPILER_SEMANTICS_H

#include "Syntactic.h"
#include "ParserTables.h"
#include <vector>

namespace Semantics{

    static std::vector<syntactic::t_attrib> stackSem;
    
    void semantics( RuleSign ruleNo, int tokenSecond);
    
    void pushSem (syntactic::t_attrib &attrib);
    void popSem(int popsNumber);
    syntactic::t_attrib topSem(int offsetFromTop);
}
#endif // COMPILER_SEMANTICS_H