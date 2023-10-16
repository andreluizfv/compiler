#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include <optional>
#include <vector>

#include "Scanner.h"

using namespace std;

class Block {
 public:
  vector<shared_ptr<Semantics::Obj>> objs;
};

class SymbolTable {
 public:
  vector<Block> blocks;
  vector<Block>::iterator newBlock();
  vector<Block>::iterator endBlock();
  SymbolTable();
  vector<shared_ptr<Semantics::Obj>>::iterator define(Semantics::Obj &obj);
  optional<vector<shared_ptr<Semantics::Obj>>::iterator> searchWhenDeclared(Semantics::Obj &obj);
  optional<vector<shared_ptr<Semantics::Obj>>::iterator> findWhenUsed(Semantics::Obj &obj);
};

#endif
