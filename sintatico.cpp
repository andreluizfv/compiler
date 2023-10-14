#include <iostream>
#include <iterator>
#include <stack>

#include "ParserTables.h"

typedef std::string string;

#define IS_SHIFT(p) ((p) > 0)
#define IS_REDUCTION(p) ((p) < 0)
#define RULE(p) (-(p))

void sintatico() {
    int q = 0; 
    std::stack<int> stack;
    stack.push(0); 
    int nterminal_shift = 35; // passando os não terminais para pegar os tokens terminais
    int FINAL = 1; // estado final da tabela (linha so com zeros)
    int a = nextToken() + nterminal_shift;
    do {
        int p = action_table[q][a];
        if( IS_SHIFT(p) ) {
            stack.push(p);
            a = nextToken() + nterminal_shift;
        } else
        if( IS_REDUCTION(p) ) {
            int r = RULE(p)-1;
            for (int i = 0; i <  aux_table[r][0]; i++) stack.pop();
            stack.push(action_table[stack.top()][aux_table[r][1]]);
        } else {
            error();
        }
        q = stack.top();
    } while( q != FINAL);
}


void error() {
  std::cout << "erro";
}
