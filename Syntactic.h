#ifndef COMPILER_SYNTACTIC_H
#define COMPILER_SYNTACTIC_H
#include "Token.h"
#include "ParserTables.h"
#include "Scope.h"
#include <vector>
namespace syntactic {

int nextToken(std::vector<Token>::const_iterator& iterator);
void error();
void syntatic_analysis(const std::vector<Token>& tokens);

typedef struct IDD {int name; Scope::scope_object* obj;} IDD;
typedef struct IDU {int name; Scope::scope_object* obj;} IDU;
typedef struct ID {int name; Scope::scope_object* obj;} ID;

class t_attrib{
    public:
        non_term nTerminal;
        std::variant<IDD, IDU, ID> _;
    };
}

#endif //COMPILER_SYNTACTIC_H