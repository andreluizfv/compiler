#ifndef COMPILER_SEMANTICS_H
#define COMPILER_SEMANTICS_H

#include "Syntatic.h"
#include <vector>

namespace Semantics{

    static std::vector<Syntatic::t_attrib> stackSem;
    
    void Semantics( Syntatic::t_rule ruleNo, int tokenSecond);
    
    void pushSem (Syntatic::t_attrib &attrib);
    void popSem(int popsNumber);
    Syntatic::t_attrib topSem(int offsetFromTop);
}
#endif // COMPILER_SEMANTICS_H