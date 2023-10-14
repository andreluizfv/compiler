#pragma once
#include <variant>
#include <stack>
#include "Scope.h"
#ifndef COMPILER_SYNTATIC_H
#define COMPILER_SYNTATIC_H
namespace Syntatic{
    typedef enum { IDD_, IDU_, ID_ } t_nont;

    typedef enum { IDD_TO_Id, IDU_TO_Id, ID_TO_Id, NewBlock_to_eps, Type_Declaration, Function_Declaration,
                   Statement_to_Block} t_rule;

    typedef struct IDD {int name; Scope::scope_object* obj;} IDD;
    typedef struct IDU {int name; Scope::scope_object* obj;} IDU;
    typedef struct ID {int name; Scope::scope_object* obj;} ID;

    class t_attrib{
    public:
        t_nont nTerminal;
        std::variant<IDD, IDU, ID> _;
        
    };
}
#endif //COMPILER_SYNTATIC_H