#pragma once
#ifndef COMPILER_SCOPE_H
#define COMPILER_SCOPE_H
#include "ParserTables.h"
#include <vector>
#include <variant>

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

    typedef struct IDD {int name; Scope::scope_object* obj;} IDD;
    typedef struct IDU {int name; Scope::scope_object* obj;} IDU;
    typedef struct ID {int name; Scope::scope_object* obj;} ID;

    class t_attrib{
    public:
        non_term nTerminal;
        std::variant<IDD, IDU, ID> _;
    };
}

#endif // COMPILER_SCOPE_H