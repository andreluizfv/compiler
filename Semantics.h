#ifndef COMPILER_SEMANTICS_H
#define COMPILER_SEMANTICS_H

#include <fstream>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "ParserTables.h"
#include "Token.h"
#include "Utils.h"

namespace Semantics {
    struct Obj;
    struct Attr;

    enum KindType {
        NO_KIND_DEF_ = -1,
        VAR_,
        PARAM_,
        FUNCTION_,
        FIELD_,
        ARRAY_TYPE_,
        STRUCT_TYPE_,
        ALIAS_TYPE_,
        SCALAR_TYPE_,
        UNIVERSAL_
    };

    struct ObjType {
        std::shared_ptr<Obj> type;
        int n_idx;
        int n_size;
    };

    struct ObjFunc {
        std::shared_ptr<Obj> ret_type;
        std::vector<std::shared_ptr<Obj>> params;
        int n_idx;
        int n_params;
        int n_vars;
    };

    struct ObjArray {
        std::shared_ptr<Obj> el_type;
        int num_el;
        int n_size;
    };

    struct ObjStruct {
        std::vector<std::shared_ptr<Obj>> fields;
        int n_size;
    };

    struct ObjAlias {
        std::shared_ptr<Obj> base_type;
        int n_size;
    };

    typedef std::variant<std::monostate, ObjType, ObjFunc,
            ObjArray, ObjStruct, ObjAlias>
            ObjVariant;

    struct Obj {
        int name;
        KindType kindType;
        ObjVariant obj;

        Obj(int name, KindType kindType, ObjVariant obj) : name(name), kindType(kindType), obj(std::move(obj)) {}
    };

    struct AttrId {
        std::shared_ptr<Obj> obj;
        int name;
    };

    struct AttrType {
        std::shared_ptr<Obj> type;
    };

    struct AttrMc {
        std::shared_ptr<Obj> type;
        std::vector<std::shared_ptr<Obj>> param;
        int err;
    };

    struct AttrLabel {
        int label;
    };

    struct AttrLe {
        std::shared_ptr<Obj> type;
        std::vector<std::shared_ptr<Obj>> param;
        int err;
        int n;
    };

    struct AttrList {
        std::vector<std::shared_ptr<Obj>> list;
    };

    struct AttrBool {
        std::shared_ptr<Obj> type;
        bool val;
    };

    struct AttrChar {
        std::shared_ptr<Obj> type;
        int pos;
        char val;
    };

    struct AttrString {
        std::shared_ptr<Obj> type;
        int pos;
        std::string val;
    };

    struct AttrNum {
        std::shared_ptr<Obj> type;
        int pos;
        int val;
    };

    typedef std::variant<std::monostate, AttrId, AttrType,
            AttrMc, AttrLabel, AttrLe,
            AttrList, AttrBool, AttrChar,
            AttrString, AttrNum>
            AttrVariant;

    struct Attr {
        int size;
        non_term type;
        AttrVariant attr;

        Attr(int size, non_term type, AttrVariant attr) : size(size), type(type), attr(std::move(attr)) {}
    };

    void addRule(int rule, int last_snd_token, int last_const_idx, int line, const std::unordered_map<int, Object> &constants,
                 bool& err, std::ofstream codeFile);
    bool isSameType(const std::shared_ptr<Obj>& t1, const std::shared_ptr<Obj>& t2);
}

#endif //COMPILER_SEMANTICS_H
