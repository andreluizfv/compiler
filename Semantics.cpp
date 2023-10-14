#include "Semantics.h"
#include "Scope.h"
#include "Utils.h"
#include "Syntatic.h"
#include <variant>
#include<stack>

using namespace Semantics;

void Semantics::Semantics( Syntatic::t_rule ruleNo, int tokenSecond = -1)
{
    Syntatic::t_attrib IDD , IDU, ID;
    switch (ruleNo){
        case Syntatic::IDD_TO_Id:{
            std::get<Syntatic::IDD>(IDD._).name = tokenSecond;
            auto p = Scope::SearchWhenDeclared(tokenSecond);
            if (p !=  Scope::SymbolTable[Scope::nCurrentLevel].end()){
                throw(Utils::RedeclaredException());
            }
            else{
                *p = Scope::Define(tokenSecond);
            }
            *std::get<Syntatic::IDD>(IDD._).obj = (*p);
        } case Syntatic::IDU_TO_Id:{
            std::get<Syntatic::IDU>(IDU._).name = tokenSecond;
            auto p = Scope::FindWhenUsed(tokenSecond);
            if (p == Scope::SymbolTable[0].end()){
                throw(Utils::NotDeclaredException());
                *p = Scope::Define(tokenSecond);
            }
            *std::get<Syntatic::IDU>(IDU._).obj = *p;
        } case Syntatic::ID_TO_Id:{
            std::get<Syntatic::ID>(ID._).name = tokenSecond;
            std::get<Syntatic::ID>(ID._).obj = NULL;
        } case Syntatic::NewBlock_to_eps:{
            Scope::NewBlock();
        } case Syntatic::Type_Declaration:{
            Scope::EndBlock();
        } case Syntatic::Function_Declaration:{
            Scope::EndBlock();
        } case Syntatic::Statement_to_Block:{
            Scope::EndBlock();
        }
    }
}

void pushSem(Syntatic::t_attrib &attrib){
    Semantics::stackSem.push_back(attrib);
}

void popSem(int popsNumber){
    for(int i = 0; i < popsNumber; i++) Semantics::stackSem.pop_back();
}
Syntatic::t_attrib topSem(int offsetFromTop){
    return *(Semantics::stackSem.end() - offsetFromTop -1);
}