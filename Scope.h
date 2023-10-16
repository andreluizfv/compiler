#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include <optional>
#include <vector>

#include "Scanner.h"

using namespace std;

struct SemaObj;
struct SemaAttr;

class Block {
 public:
  vector<shared_ptr<SemaObj>> objs;
};

class SymbolTable {
 public:
  vector<Block> blocks;
  vector<Block>::iterator newBlock();
  vector<Block>::iterator endBlock();
  SymbolTable();
  vector<shared_ptr<SemaObj>>::iterator define(SemaObj &obj);
  optional<vector<shared_ptr<SemaObj>>::iterator> search(SemaObj &obj);
  optional<vector<shared_ptr<SemaObj>>::iterator> find(SemaObj &obj);
};

#endif
