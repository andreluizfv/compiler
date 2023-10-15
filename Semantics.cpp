#include "Semantics.h"
#include "Scope.h"
#include "Utils.h"
#include "ParserTables.h"
#include <variant>
#include <stack>

using namespace Semantics;

void Semantics::semantics(RuleSign ruleNo, int tokenSecond){
    Scope::t_attrib IDD , IDU, ID;
    switch (ruleNo){
        case R_IDD_0:{
            std::get<Scope::IDD>(IDD._).name = tokenSecond;
            auto p = Scope::SearchWhenDeclared(tokenSecond);
            if (p !=  Scope::SymbolTable[Scope::nCurrentLevel].end()){
                throw(Utils::RedeclaredException());
            }
            else{
                *p = Scope::Define(tokenSecond);
            }
            *std::get<Scope::IDD>(IDD._).obj = (*p);
        } case R_IDU_0:{
            std::get<Scope::IDU>(IDU._).name = tokenSecond;
            auto p = Scope::FindWhenUsed(tokenSecond);
            if (p == Scope::SymbolTable[0].end()){
                throw(Utils::NotDeclaredException());
                *p = Scope::Define(tokenSecond);
            }
            *std::get<Scope::IDU>(IDU._).obj = *p;
        }
        // case syntactic::ID_TO_Id:{
        //     std::get<syntactic::ID>(ID._).name = tokenSecond;
        //     std::get<syntactic::ID>(ID._).obj = NULL;
        // }
        case R_NB_0:
            Scope::NewBlock();
         case R_DT_1:
         case R_DF_0:
         case R_S_4:
            Scope::EndBlock();

    }
}

void pushSem(Scope::t_attrib &attrib){
    Semantics::stackSem.push_back(attrib);
}

void popSem(int popsNumber){
    for(int i = 0; i < popsNumber; i++) Semantics::stackSem.pop_back();
}
Scope::t_attrib topSem(int offsetFromTop){
    return *(Semantics::stackSem.end() - offsetFromTop -1);
}