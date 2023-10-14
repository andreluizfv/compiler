#include <iostream>
#include <iterator>
#include <stack>
#include <vector>

#include "ParserTables.h"
#include "Token.h"
#include "Semantics.h"
namespace syntactic {
#define IS_SHIFT(p) ((p) > 0)
#define IS_REDUCTION(p) ((p) < 0)
#define RULE(p) (-(p))

    int nextToken(std::vector<Token>::const_iterator& iterator) {
        auto curr = *iterator;
        std::advance(iterator, 1);
        return curr.type;
    }

    void error() {
        std::cout << "error";
    }

    void syntatic_analysis(const std::vector<Token>& tokens) {
        int q = 0;
        std::stack<int> stack;
        stack.push(0);
        int nterminal_shift = 35; // passando os não terminais para pegar os tokens terminais
        int FINAL = 1; // estado final da tabela (linha so com zeros)
        auto itr = tokens.begin();
        int a = nextToken(itr) + nterminal_shift;

        do {
            int p = action_table[q][a];
            if( IS_SHIFT(p) ) {
                stack.push(p);
                a = nextToken(itr) + nterminal_shift;
            } else
            if( IS_REDUCTION(p) ) {
                int r = RULE(p)-1;
                for (int i = 0; i <  aux_table[r][0]; i++) stack.pop();
                stack.push(action_table[stack.top()][aux_table[r][1]]);
                Semantics::Semantics(static_cast<RuleSign>(r), itr->tokenSecond);
            } else {
                error();
            }
            q = stack.top();
        } while( q != FINAL);
    }

} // namespace syntactic