#pragma once
#include <variant>
#include "Scope.h"
#ifndef COMPILER_SYNTATIC_H
#define COMPILER_SYNTATIC_H
namespace Syntatic{
    typedef enum { IDD_, IDU_, ID_ } t_nont;

    typedef enum { IDD_TO_Id, IDU_TO_Id, ID_TO_Id } t_rule;

    typedef struct IDD {int* name; Scope::scope_object* obj;} IDD;
    typedef struct IDD {int* name; Scope::scope_object* obj;} IDU;
    typedef struct ID {int* name; Scope::scope_object* obj;} IDU;

    class t_attrib{
    public:
        std::variant<IDD,IDU, ID> _;
        
    };
}
#endif //COMPILER_SYNTATIC_H