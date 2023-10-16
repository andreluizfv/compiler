#include <iostream>
#include <iterator>
#include <stack>
#include <vector>

#include "ParserTables.h"
#include "Syntactic.h"
#include "Token.h"
#include "Utils.h"
#include "Semantics.h"
#define IS_SHIFT(p) ((p) > 0)
#define IS_REDUCTION(p) ((p) < 0)
#define RULE(p) (-(p))

int Syntactic::nextToken(std::vector<Token>::const_iterator& iterator) {
    auto curr = *iterator;
    std::advance(iterator, 1);
    return curr.type;
}

void Syntactic::error(const Token& token, int idx) {
    std::cout << "Syntactic error for " << token.toString() << "at index " << idx;
}

Syntactic::Syntactic(const std::vector<Token>& tokens) {
    int last_snd_token = -1;
    int last_const_idx = -1;
    auto itr = tokens.begin();
    std::stack<int> stack;

    stack.push(0);
    int q = 0;
    int nterminal_shift = 35; // passando os não terminais para pegar os tokens terminais
    int FINAL = 1; // estado final da tabela (linha so com zeros)
    int a = nextToken(itr) + nterminal_shift;
    do {
        if (itr->tokenSecond != -1) last_snd_token = itr->tokenSecond;
        if (itr->const_idx != -1) {
            last_const_idx = itr->const_idx;
            Utils::Literals::constantsMap[last_const_idx] = itr->literal;
        }


        int p = action_table[q][a];
        if( IS_SHIFT(p) ) {
            stack.push(p);
            a = nextToken(itr) + nterminal_shift;
        } else
        if( IS_REDUCTION(p) ) {
            int r = RULE(p)-1;
            if (r == 0) break;
            bool err = false;
            Semantics::addRule(r, last_snd_token, last_const_idx, -1, Utils::Literals::constantsMap, err);
            if (err) break;

            for (int i = 0; i <  aux_table[r][0]; i++) stack.pop();
            stack.push(action_table[stack.top()][aux_table[r][1]]);
        } else {
            error(*itr, itr - tokens.begin());
            break;
        }
        q = stack.top();
    } while(q != FINAL or itr->type != UNKNOWN or itr != this->tokens.end());
}