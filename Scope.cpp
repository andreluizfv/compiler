#include "Scope.h"

#include <iostream>
#include <optional>

#include "Semantics.h"


namespace Scope {
    SymbolTable::SymbolTable() {
        blocks.emplace_back();
    }

    std::vector<Block>::iterator SymbolTable::newBlock() {
        blocks.emplace_back();
        return blocks.end() - 1;
    }

    std::vector<Block>::iterator SymbolTable::endBlock() {
        blocks.pop_back();
        return blocks.end() - 1;
    }

    std::vector<std::shared_ptr<Semantics::Obj>>::iterator SymbolTable::define(Semantics::Obj &obj) {
        auto last_block = blocks.end() - 1;
        last_block->objs.push_back(std::make_shared<Semantics::Obj>(obj));
        return last_block->objs.end() - 1;
    }

    std::optional<std::vector<std::shared_ptr<Semantics::Obj>>::iterator> SymbolTable::searchWhenDeclared(Semantics::Obj &obj) {
        if (blocks.empty())
            return std::nullopt;

        auto last_block = blocks.end() - 1;
        for (auto itr = last_block->objs.begin(); itr < last_block->objs.end(); std::advance(itr, 1)) {
            if ((*itr)->name == obj.name) return itr;
        }

        return std::nullopt;
    }

    std::optional<std::vector<std::shared_ptr<Semantics::Obj>>::iterator> SymbolTable::findWhenUsed(Semantics::Obj &obj) {
        for (auto block = blocks.rbegin(); block < blocks.rend(); block++)
            for (auto curr_obj = block->objs.begin(); curr_obj < block->objs.end(); std::advance(curr_obj, 1))
                if ((*curr_obj)->name == obj.name)
                    return curr_obj;

        return std::nullopt;
    }
}
