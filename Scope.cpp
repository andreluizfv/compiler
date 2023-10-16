#include "Scope.h"

#include <iostream>
#include <optional>

#include "Semantics.h"

using namespace std;

SymbolTable::SymbolTable() { blocks.push_back(Block()); }

vector<Block>::iterator SymbolTable::newBlock() {
  blocks.push_back(Block());
  return blocks.end() - 1;
}

vector<Block>::iterator SymbolTable::endBlock() {
  blocks.pop_back();
  return blocks.end() - 1;
}

vector<shared_ptr<Semantics::Obj>>::iterator SymbolTable::define(Semantics::Obj &obj) {
  auto last_block = blocks.end() - 1;
  last_block->objs.push_back(make_shared<Semantics::Obj>(obj));
  // cout << "DEFINED : " << obj.name << endl;

  return last_block->objs.end() - 1;
}

optional<vector<shared_ptr<Semantics::Obj>>::iterator> SymbolTable::searchWhenDeclared(
        Semantics::Obj &obj) {
  if (blocks.size() == 0) return nullopt;

  auto last_block = blocks.end() - 1;
  for (auto itr = last_block->objs.begin(); itr < last_block->objs.end();
       itr++) {
    if ((*itr)->name == obj.name) return itr;
  }

  return nullopt;
}

optional<vector<shared_ptr<Semantics::Obj>>::iterator> SymbolTable::findWhenUsed(
        Semantics::Obj &obj) {
  for (auto block = blocks.rbegin(); block < blocks.rend(); block++)
    for (auto curr_obj = block->objs.begin(); curr_obj < block->objs.end();
         curr_obj++)
      if ((*curr_obj)->name == obj.name) return curr_obj;

  return nullopt;
}
