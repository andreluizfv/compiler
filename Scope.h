#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include <optional>
#include <vector>

#include "Scanner.h"
#include "Semantics.h"

namespace Scope {
    class Block {
    public:
        std::vector<std::shared_ptr<Semantics::Obj>> objs;
    };

    class SymbolTable {
    public:
        SymbolTable();

        std::vector<Block> blocks;

        std::vector<Block>::iterator newBlock();

        std::vector<Block>::iterator endBlock();

        std::vector<std::shared_ptr<Semantics::Obj>>::iterator define(Semantics::Obj &obj);

        std::optional<std::vector<std::shared_ptr<Semantics::Obj>>::iterator> searchWhenDeclared(Semantics::Obj &obj);

        std::optional<std::vector<std::shared_ptr<Semantics::Obj>>::iterator> findWhenUsed(Semantics::Obj &obj);
    };
}
#endif // SCOPE_H