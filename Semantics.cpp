#include <iostream>
#include "Semantics.h"
#include "Scope.h"


#define IS_TYPE_KIND(k) ((k) == ARRAY_TYPE_ or (k) == STRUCT_TYPE_ or (k) == ALIAS_TYPE_ or (k) == SCALAR_TYPE_)
namespace {
    std::shared_ptr<Semantics::Obj> int_t =
            std::make_shared<Semantics::Obj>(-1, Semantics::SCALAR_TYPE_, std::monostate());
    std::shared_ptr<Semantics::Obj> bool_t =
            std::make_shared<Semantics::Obj>(-1, Semantics::SCALAR_TYPE_, std::monostate());
    std::shared_ptr<Semantics::Obj> char_t =
            std::make_shared<Semantics::Obj>(-1, Semantics::SCALAR_TYPE_, std::monostate());
    std::shared_ptr<Semantics::Obj> string_t =
            std::make_shared<Semantics::Obj>(-1, Semantics::SCALAR_TYPE_, std::monostate());
    std::shared_ptr<Semantics::Obj> universal_t =
            std::make_shared<Semantics::Obj>(-1, Semantics::SCALAR_TYPE_, std::monostate());

    void error(int line, std::string errorMessage) {
            std::cout << "error at " << line << " message: "  << errorMessage;
    }
}

namespace Semantics {
    bool isSameType(const std::shared_ptr<Obj>& type1, const std::shared_ptr<Obj>& type2) {
        if (type1 == type2 or type1 == universal_t or type2 == universal_t)
            return true;
        if (type1->kindType == UNIVERSAL_ or type2->kindType == UNIVERSAL_)
            return true;
        if (type1->kindType == ALIAS_TYPE_ and type2->kindType != ALIAS_TYPE_)
            return isSameType(std::get<ObjAlias>(type1->obj).base_type, type2);
        if (type1->kindType != ALIAS_TYPE_ and type2->kindType == ALIAS_TYPE_)
            return isSameType(type1, std::get<ObjAlias>(type2->obj).base_type);
        if (type1->kindType == type2->kindType) {
            if (type1->kindType == ALIAS_TYPE_) {
                return isSameType(std::get<ObjAlias>(type1->obj).base_type,
                                   std::get<ObjAlias>(type2->obj).base_type);
            } else if (type1->kindType == ARRAY_TYPE_) {
                if (std::get<ObjArray>(type1->obj).num_el ==
                    std::get<ObjArray>(type2->obj).num_el) {
                    return isSameType(std::get<ObjArray>(type1->obj).el_type,
                                       std::get<ObjArray>(type2->obj).el_type);
                }
            } else if (type1->kindType == STRUCT_TYPE_) {
                auto f1 = std::get<ObjStruct>(type1->obj).fields;
                auto f2 = std::get<ObjStruct>(type2->obj).fields;

                auto f1_iter = f1.begin();
                auto f2_iter = f2.begin();
                while (f1_iter != f1.end() and f2_iter != f2.end()) {
                    if (!isSameType(std::get<ObjType>((*f1_iter)->obj).type,
                                     std::get<ObjType>((*f2_iter)->obj).type))
                        return false;
                }
                return (f1_iter == f1.end() and f2_iter == f2.end());
            }
        }
        return false;
    }

    void addRule(int rule, int last_snd_token, int last_const_idx, int line,
                            std::unordered_map<int, Object> &constants) {
        static std::shared_ptr<Obj> curr_func;
        Scope::SymbolTable symtable;
        std::stack<Attr> semantic_stack;

        static int num_funcs = 0;
        static int curr_label = 0;
        static int func_var_pos = 0;

        switch (rule) {
            case R_LDE_0:
            case R_LDE_1:
            case R_DE_0:
            case R_DE_1:
                break;

            case R_T_0:
                semantic_stack.emplace(1, _T, AttrType{int_t});
                break;

            case R_T_1:
                semantic_stack.emplace(1, _T, AttrType{char_t});
                break;

            case R_T_2:
                semantic_stack.emplace(1, _T, AttrType{bool_t});
                break;

            case R_T_3:
                semantic_stack.emplace(1, _T, AttrType{string_t});
                break;

            case R_T_4: {
                auto idu = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrId>(idu.attr).obj;
                Attr t(0, _T, AttrType{p});

                if (IS_TYPE_KIND(p->kindType) or p->kindType == UNIVERSAL_) {
                    if (p->kindType == ALIAS_TYPE_)
                        t.size = std::get<ObjAlias>(p->obj).n_size;
                    else if (p->kindType == ARRAY_TYPE_)
                        t.size = std::get<ObjArray>(p->obj).n_size;
                    else if (p->kindType == STRUCT_TYPE_)
                        t.size = std::get<ObjStruct>(p->obj).n_size;
                } else {
                    t.attr = AttrType{universal_t};
                    t.size = 0;
                    error(line, "Sema error: Type expected");
                }
                semantic_stack.push(t);
                break;
            }

            case R_DT_0: {
                auto t_nonterm = semantic_stack.top();
                semantic_stack.pop();
                auto num = semantic_stack.top();
                semantic_stack.pop();
                auto idd = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrId>(idd.attr).obj;
                auto n = std::get<AttrNum>(num.attr).val;
                auto t = std::get<AttrType>(t_nonterm.attr).type;

                p->kindType = ARRAY_TYPE_;
                ObjArray obj = {
                        .el_type = t, .num_el = n, .n_size = n * t_nonterm.size};
                p->obj = obj;
                break;
            }

            case R_DT_1: {
                auto dc = semantic_stack.top();
                semantic_stack.pop();
                auto idd = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrId>(idd.attr).obj;
                auto dc_list = std::get<AttrList>(dc.attr).list;

                p->kindType = STRUCT_TYPE_;
                ObjStruct obj = {.fields = dc_list, .n_size = dc.size};
                p->obj = obj;

                symtable.endBlock();
                break;
            }

            case R_DT_2: {
                auto t_nonterm = semantic_stack.top();
                semantic_stack.pop();
                auto idd = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrId>(idd.attr).obj;
                auto t = std::get<AttrType>(t_nonterm.attr).type;

                p->kindType = ALIAS_TYPE_;
                ObjAlias obj = {.base_type = t, .n_size = t_nonterm.size};
                p->obj = obj;
                break;
            }

            case R_DC_0: {
                auto t_nonterm = semantic_stack.top();
                semantic_stack.pop();
                auto li = semantic_stack.top();
                semantic_stack.pop();
                auto dc1 = semantic_stack.top();
                semantic_stack.pop();

                auto p_list = std::get<AttrList>(li.attr).list;
                auto dc1_list = std::get<AttrList>(dc1.attr).list;
                auto t = std::get<AttrType>(t_nonterm.attr).type;
                auto n = dc1.size;

                for (auto &p : p_list) {
                    if (p->kindType != NO_KIND_DEF_) break;

                    p->kindType = FIELD_;
                    ObjType obj = {
                            .type = t, .n_idx = n, .n_size = t_nonterm.size};
                    n += t_nonterm.size;
                    p->obj = obj;
                }

                auto dc0 = Attr(n, _DC, AttrList{dc1_list});
                semantic_stack.push(dc0);
                break;
            }

            case R_DC_1: {
                auto t_nonterm = semantic_stack.top();
                semantic_stack.pop();
                auto li = semantic_stack.top();
                semantic_stack.pop();

                auto p_list = std::get<AttrList>(li.attr).list;
                auto t = std::get<AttrType>(t_nonterm.attr).type;
                auto n = 0;

                for (auto &p : p_list) {
                    if (p->kindType != NO_KIND_DEF_) break;

                    p->kindType = FIELD_;
                    ObjType obj = {
                            .type = t, .n_idx = n, .n_size = t_nonterm.size};
                    n += t_nonterm.size;
                    p->obj = obj;
                }

                auto dc = Attr(n, _DC, AttrList{p_list});
                semantic_stack.push(dc);
                break;
            }

            case R_DF_0:
                symtable.endBlock();
                break;

            case R_LP_0: {
                auto t_nonterm = semantic_stack.top();
                semantic_stack.pop();
                auto idd = semantic_stack.top();
                semantic_stack.pop();
                auto lp1 = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrId>(idd.attr).obj;
                auto t = std::get<AttrType>(t_nonterm.attr).type;
                auto lp0_list = std::get<AttrList>(lp1.attr).list;
                auto n = lp1.size;

                p->kindType = PARAM_;
                ObjType obj = {.type = t, .n_idx = n, .n_size = t_nonterm.size};
                p->obj = obj;

                auto lp0 = Attr(n + t_nonterm.size, _LP, AttrList{lp0_list});
                semantic_stack.push(lp0);
                break;
            }

            case R_LP_1: {
                auto t_nonterm = semantic_stack.top();
                semantic_stack.pop();
                auto idd = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrId>(idd.attr).obj;
                auto t = std::get<AttrType>(t_nonterm.attr).type;

                p->kindType = PARAM_;
                ObjType obj = {.type = t, .n_idx = 0, .n_size = t_nonterm.size};
                p->obj = obj;

                auto lp = Attr(t_nonterm.size, _LP, AttrList{{p}});
                semantic_stack.push(lp);
                break;
            }

            case R_B_0:
            case R_LDV_0:
            case R_LDV_1:
            case R_LS_0:
            case R_LS_1:
                break;

            case R_DV_0: {
                auto t_nonterm = semantic_stack.top();
                semantic_stack.pop();
                auto li = semantic_stack.top();
                semantic_stack.pop();

                auto p_list = std::get<AttrList>(li.attr).list;
                auto t = std::get<AttrType>(t_nonterm.attr).type;
                auto n = std::get<ObjFunc>(curr_func->obj).n_vars;

                for (auto &p : p_list) {
                    if (p->kindType != NO_KIND_DEF_) break;

                    p->kindType = VAR_;
                    ObjType obj = {
                            .type = t, .n_idx = n, .n_size = t_nonterm.size};
                    n += t_nonterm.size;
                    p->obj = obj;
                }

                auto new_obj = std::get<ObjFunc>(curr_func->obj);
                new_obj.n_vars = n;
                curr_func->obj = new_obj;
                break;
            }

            case R_LI_0: {
                auto idd = semantic_stack.top();
                semantic_stack.pop();
                auto li1 = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrList>(li1.attr).list;
                p.push_back(std::get<AttrId>(idd.attr).obj);

                auto li0 = Attr(-1, _LI, AttrList{p});
                semantic_stack.push(li0);
                break;
            }

            case R_LI_1: {
                auto idd = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrId>(idd.attr).obj;

                auto li = Attr(-1, _LI, AttrList{{p}});
                semantic_stack.push(li);
                break;
            }

            case R_MT_0: {
                int l = curr_label++;
                auto mt_variant = Attr(0, _MT, AttrLabel{l});
                semantic_stack.push(mt_variant);

                break;
            }

            case R_S_0: {
                auto me = semantic_stack.top();
                semantic_stack.pop();
                auto mt = semantic_stack.top();
                semantic_stack.pop();
                auto e = semantic_stack.top();
                semantic_stack.pop();

                int l = std::get<AttrLabel>(me.attr).label;
                auto t = std::get<AttrType>(e.attr).type;

                if (!isSameType(t, bool_t)) error(line, "Sema error: Invalid type");

                break;
            }

            case R_S_1: {
                auto mt = semantic_stack.top();
                semantic_stack.pop();
                auto e = semantic_stack.top();
                semantic_stack.pop();

                int l = std::get<AttrLabel>(mt.attr).label;
                auto t = std::get<AttrType>(e.attr).type;

                if (!isSameType(t, bool_t)) error(line, "Sema error: Invalid type");

                break;
            }

            case R_ME_0: {
                auto mt = semantic_stack.top();

                int l1 = std::get<AttrLabel>(mt.attr).label;
                int l2 = curr_label++;

                auto me = Attr(-1, _ME, AttrLabel{l2});
                semantic_stack.push(me);

                break;
            }

            case R_MW_0: {
                int l = curr_label++;

                auto mw = Attr(-1, _MW, AttrLabel{l});
                semantic_stack.push(mw);

                break;
            }

            case R_S_2: {
                auto mt = semantic_stack.top();
                semantic_stack.pop();
                auto e = semantic_stack.top();
                semantic_stack.pop();
                auto mw = semantic_stack.top();
                semantic_stack.pop();

                int l1 = std::get<AttrLabel>(mw.attr).label;
                int l2 = std::get<AttrLabel>(mt.attr).label;
                auto t = std::get<AttrType>(e.attr).type;

                if (!isSameType(t, bool_t)) error(line, "Sema error: Invalid type");

                break;
            }

            case R_S_3: {
                auto e = semantic_stack.top();
                semantic_stack.pop();
                auto mw = semantic_stack.top();
                semantic_stack.pop();

                int l = std::get<AttrLabel>(mw.attr).label;
                auto t = std::get<AttrType>(e.attr).type;

                if (!isSameType(t, bool_t)) error(line, "Sema error: Invalid type");

                break;
            }

            case R_S_4: {
                symtable.endBlock();
                break;
            }

            case R_S_5: {
                auto e = semantic_stack.top();
                semantic_stack.pop();
                auto lv = semantic_stack.top();
                semantic_stack.pop();

                auto t = std::get<AttrType>(lv.attr).type;

                if (!isSameType(std::get<AttrType>(e.attr).type, std::get<AttrType>(lv.attr).type))
                    error(line, "Sema error: Type mismatch");

                break;
            }

            case R_S_6:
            case R_S_7:
                break;
            case R_S_8: {
                auto e = semantic_stack.top();
                semantic_stack.pop();

                if (!isSameType(std::get<AttrType>(e.attr).type, std::get<ObjFunc>(curr_func->obj).ret_type))
                    error(line, "Sema error: Type mismatch");

                break;
            }

            case R_E_0: {
                auto l = semantic_stack.top();
                semantic_stack.pop();
                auto e1 = semantic_stack.top();
                semantic_stack.pop();

                if (!isSameType(std::get<AttrType>(l.attr).type, bool_t))
                    error(line, "Sema error: Invalid type");
                if (!isSameType(std::get<AttrType>(e1.attr).type, bool_t))
                    error(line, "Sema error: Invalid type");

                auto e0 = Attr(-1, _E, AttrType{bool_t});
                semantic_stack.push(e0);

                break;
            }

            case R_E_1: {
                auto l = semantic_stack.top();
                semantic_stack.pop();
                auto e1 = semantic_stack.top();
                semantic_stack.pop();

                if (!isSameType(std::get<AttrType>(l.attr).type, bool_t))
                    error(line, "Sema error: Invalid type");
                if (!isSameType(std::get<AttrType>(e1.attr).type, bool_t))
                    error(line, "Sema error: Invalid type");

                auto e0 = Attr(-1, _E, AttrType{bool_t});
                semantic_stack.push(e0);

                break;
            }

            case R_E_2: {
                auto l = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(l.attr).type;

                auto e = Attr(-1, _E, AttrType{p});
                semantic_stack.push(e);
                break;
            }

            case R_L_0: {
                auto r = semantic_stack.top();
                semantic_stack.pop();
                auto l1 = semantic_stack.top();
                semantic_stack.pop();

                if (!isSameType(std::get<AttrType>(r.attr).type,std::get<AttrType>(l1.attr).type))
                    error(line, "Sema error: Type mismatch");

                auto l0 = Attr(-1, _L, AttrType{bool_t});
                semantic_stack.push(l0);

                break;
            }

            case R_L_1: {
                auto r = semantic_stack.top();
                semantic_stack.pop();
                auto l1 = semantic_stack.top();
                semantic_stack.pop();

                if (!isSameType(std::get<AttrType>(r.attr).type,std::get<AttrType>(l1.attr).type))
                    error(line, "Sema error: Type mismatch");

                auto l0 = Attr(-1, _L, AttrType{bool_t});
                semantic_stack.push(l0);

                break;
            }

            case R_L_2: {
                auto r = semantic_stack.top();
                semantic_stack.pop();
                auto l1 = semantic_stack.top();
                semantic_stack.pop();

                if (!isSameType(std::get<AttrType>(r.attr).type,std::get<AttrType>(l1.attr).type))
                    error(line, "Sema error: Type mismatch");

                auto l0 = Attr(-1, _L, AttrType{bool_t});
                semantic_stack.push(l0);

                break;
            }

            case R_L_3: {
                auto r = semantic_stack.top();
                semantic_stack.pop();
                auto l1 = semantic_stack.top();
                semantic_stack.pop();

                if (!isSameType(std::get<AttrType>(r.attr).type,std::get<AttrType>(l1.attr).type))
                    error(line, "Sema error: Type mismatch");

                auto l0 = Attr(-1, _L, AttrType{bool_t});
                semantic_stack.push(l0);

                break;
            }

            case R_L_4: {
                auto r = semantic_stack.top();
                semantic_stack.pop();
                auto l1 = semantic_stack.top();
                semantic_stack.pop();

                if (!isSameType(std::get<AttrType>(r.attr).type,std::get<AttrType>(l1.attr).type))
                    error(line, "Sema error: Type mismatch");

                auto l0 = Attr(-1, _L, AttrType{bool_t});
                semantic_stack.push(l0);

                break;
            }

            case R_L_5: {
                auto r = semantic_stack.top();
                semantic_stack.pop();
                auto l1 = semantic_stack.top();
                semantic_stack.pop();

                if (!isSameType(std::get<AttrType>(r.attr).type,std::get<AttrType>(l1.attr).type))
                    error(line, "Sema error: Type mismatch");

                auto l0 = Attr(-1, _L, AttrType{bool_t});
                semantic_stack.push(l0);

                break;
            }

            case R_L_6: {
                auto r = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(r.attr).type;

                auto l0 = Attr(-1, _L, AttrType{p});
                semantic_stack.push(l0);
                break;
            }

            case R_R_0: {
                auto y = semantic_stack.top();
                semantic_stack.pop();
                auto r1 = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(r1.attr).type;

                if (!isSameType(std::get<AttrType>(y.attr).type,std::get<AttrType>(r1.attr).type))
                    error(line, "Sema error: Type mismatch");

                if (!isSameType(std::get<AttrType>(r1.attr).type, int_t) and
                    !isSameType(std::get<AttrType>(r1.attr).type, string_t))
                    error(line, "Sema error: Invalid type");

                auto r0 = Attr(-1, _R, AttrType{p});
                semantic_stack.push(r0);

                break;
            }

            case R_R_1: {
                auto y = semantic_stack.top();
                semantic_stack.pop();
                auto r1 = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(r1.attr).type;

                if (!isSameType(std::get<AttrType>(y.attr).type,std::get<AttrType>(r1.attr).type))
                    error(line, "Sema error: Type mismatch");

                if (!isSameType(std::get<AttrType>(r1.attr).type, int_t))
                    error(line, "Sema error: Invalid type");

                auto r0 = Attr(-1, _R, AttrType{p});
                semantic_stack.push(r0);

                break;
            }

            case R_R_2: {
                auto y = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(y.attr).type;

                auto r0 = Attr(-1, _R, AttrType{p});
                semantic_stack.push(r0);
                break;
            }

            case R_Y_0: {
                auto f = semantic_stack.top();
                semantic_stack.pop();
                auto y = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(y.attr).type;

                if (!isSameType(std::get<AttrType>(y.attr).type,
                                 std::get<AttrType>(f.attr).type))
                    error(line, "Sema error: Type mismatch");

                if (!isSameType(std::get<AttrType>(y.attr).type, int_t))
                    error(line, "Sema error: Invalid type");

                auto y0 = Attr(-1, _Y, AttrType{p});
                semantic_stack.push(y0);

                break;
            }

            case R_Y_1: {
                auto f = semantic_stack.top();
                semantic_stack.pop();
                auto y = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(y.attr).type;

                if (!isSameType(std::get<AttrType>(y.attr).type,
                                 std::get<AttrType>(f.attr).type))
                    error(line, "Sema error: Type mismatch");

                if (!isSameType(std::get<AttrType>(y.attr).type, int_t))
                    error(line, "Sema error: Invalid type");

                auto y0 = Attr(-1, _Y, AttrType{p});
                semantic_stack.push(y0);

                break;
            }

            case R_Y_2: {
                auto f = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(f.attr).type;

                auto y0 = Attr(-1, _Y, AttrType{p});
                semantic_stack.push(y0);
                break;
            }

            case R_F_0: {
                auto lv = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(lv.attr).type;
                auto n = std::get<ObjAlias>(p->obj).n_size;

                auto f = Attr(-1, _F, AttrType{p});
                semantic_stack.push(f);

                break;
            }

            case R_F_1: {
                auto lv = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(lv.attr).type;

                if (!isSameType(std::get<AttrType>(lv.attr).type, int_t))
                    error(line, "Sema error: Invalid type");

                auto f = Attr(-1, _F, AttrType{int_t});
                semantic_stack.push(f);

                break;
            }

            case R_F_2: {
                auto lv = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(lv.attr).type;

                if (!isSameType(std::get<AttrType>(lv.attr).type, int_t))
                    error(line, "Sema error: Invalid type");

                auto f = Attr(-1, _F, AttrType{int_t});
                semantic_stack.push(f);

                break;
            }

            case R_F_3: {
                auto lv = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(lv.attr).type;

                if (!isSameType(std::get<AttrType>(lv.attr).type, int_t))
                    error(line, "Sema error: Invalid type");

                auto f = Attr(-1, _F, AttrType{int_t});
                semantic_stack.push(f);

                break;
            }

            case R_F_4: {
                auto lv = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(lv.attr).type;

                if (!isSameType(std::get<AttrType>(lv.attr).type, int_t))
                    error(line, "Sema error: Invalid type");

                auto f = Attr(-1, _F, AttrType{int_t});
                semantic_stack.push(f);

                break;
            }

            case R_F_5: {
                auto e = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrType>(e.attr).type;

                auto f = Attr(-1, _F, AttrType{p});
                semantic_stack.push(f);
                break;
            }

            case R_F_6: {
                auto le = semantic_stack.top();
                semantic_stack.pop();
                auto mc = semantic_stack.top();
                semantic_stack.pop();
                auto idu = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrId>(idu.attr).obj;
                auto mc_type = std::get<AttrMc>(mc.attr).type;

                auto f = Attr(-1, _F, AttrType{mc_type});
                semantic_stack.push(f);

                break;
            }

            case R_F_7: {
                auto f1 = semantic_stack.top();
                semantic_stack.pop();

                auto t = std::get<AttrType>(f1.attr).type;

                if (!isSameType(std::get<AttrType>(f1.attr).type, int_t))
                    error(line, "Sema error: Invalid type");

                auto f = Attr(-1, _F, AttrType{t});
                semantic_stack.push(f);

                break;
            }

            case R_F_8: {
                auto f1 = semantic_stack.top();
                semantic_stack.pop();

                auto t = std::get<AttrType>(f1.attr).type;

                if (!isSameType(std::get<AttrType>(f1.attr).type, int_t))
                    error(line, "Sema error: Invalid type");

                auto f = Attr(-1, _F, AttrType{t});
                semantic_stack.push(f);

                break;
            }

            case R_F_9: {
                auto true_nontem = semantic_stack.top();
                semantic_stack.pop();

                auto f = Attr(-1, _F, AttrType{bool_t});
                semantic_stack.push(f);

                break;
            }

            case R_F_10: {
                auto false_nontem = semantic_stack.top();
                semantic_stack.pop();

                auto f = Attr(-1, _F, AttrType{bool_t});
                semantic_stack.push(f);

                break;
            }

            case R_F_11: {
                auto char_nontem = semantic_stack.top();
                semantic_stack.pop();

                auto f = Attr(-1, _F, AttrType{char_t});
                semantic_stack.push(f);

                break;
            }

            case R_F_12: {
                auto string_nontem = semantic_stack.top();
                semantic_stack.pop();

                auto f = Attr(-1, _F, AttrType{string_t});
                semantic_stack.push(f);

                break;
            }

            case R_F_13: {
                auto num_nontem = semantic_stack.top();
                semantic_stack.pop();

                auto f = Attr(-1, _F, AttrType{int_t});
                semantic_stack.push(f);

                break;
            }

            case R_LE_0: {
                auto e = semantic_stack.top();
                semantic_stack.pop();
                auto le1 = semantic_stack.top();
                semantic_stack.pop();

                auto le1_attr = std::get<AttrLe>(le1.attr);
                auto le0_variant =
                        AttrLe{.type = nullptr, .param = {}, .err = false, .n = 0};
                if (!le1_attr.err) {
                    if (le1_attr.param.empty()) {
                        error(line, "Sema error: To many arguments");
                        le0_variant.err = true;
                    } else {
                        if (!isSameType(
                                std::get<ObjType>(le1_attr.param[0]->obj).type,
                                std::get<AttrType>(e.attr).type))
                            error(line, "Sema error: Invalid type");

                        le0_variant.param = std::vector<std::shared_ptr<Obj>>(
                                le1_attr.param.begin() + 1, le1_attr.param.end());
                        le0_variant.n = le1_attr.n + 1;
                    }
                }

                auto le0 = Attr(-1, _LE, le0_variant);
                semantic_stack.push(le0);
                break;
            }

            case R_LE_1: {
                auto e = semantic_stack.top();
                semantic_stack.pop();
                auto mc = semantic_stack.top();
                semantic_stack.pop();

                // Stuff it back MC
                semantic_stack.push(mc);

                auto mc_attr = std::get<AttrMc>(mc.attr);
                auto le_variant = AttrLe{
                        .type = nullptr, .param = {}, .err = mc_attr.err, .n = 0};
                int n = 1;
                if (!mc_attr.err) {
                    if (mc_attr.param.empty()) {
                        error(line, "Sema error: To many arguments");
                        le_variant.err = true;
                    } else {
                        if (!isSameType(
                                std::get<ObjType>(mc_attr.param[0]->obj).type,
                                std::get<AttrType>(e.attr).type))
                            error(line, "Sema error: Invalid type");

                        le_variant.param = std::vector<std::shared_ptr<Obj>>(
                                mc_attr.param.begin() + 1, mc_attr.param.end());
                        le_variant.n = n + 1;
                    }
                }

                auto le = Attr(-1, _LE, le_variant);
                semantic_stack.push(le);
                break;
            }

            case R_LV_0: {
                auto id = semantic_stack.top();
                semantic_stack.pop();
                auto lv1 = semantic_stack.top();
                semantic_stack.pop();

                auto t = std::get<AttrType>(lv1.attr).type;
                auto lv_variant = AttrType{.type = nullptr};

                if (t->kindType != STRUCT_TYPE_) {
                    if (t->kindType != UNIVERSAL_) {
                        error(line, "Sema error: Kind not struct");
                    }
                    lv_variant.type = universal_t;
                } else {
                    auto struct_fields = std::get<ObjStruct>(t->obj).fields;
                    auto p = struct_fields.begin();

                    while (p != struct_fields.end()) {
                        if ((*p)->name == std::get<AttrId>(id.attr).name) break;

                        p++;
                    }

                    if (p == struct_fields.end()) {
                        lv_variant.type = universal_t;
                        error(line, "Sema error: Field not declared");
                    } else {
                        lv_variant.type = std::get<ObjType>((*p)->obj).type;

                        std::shared_ptr<Obj> base_type = nullptr;
                        if (std::holds_alternative<ObjAlias>(lv_variant.type->obj))
                            base_type =
                                    std::get<ObjAlias>(lv_variant.type->obj).base_type;

                        ObjAlias type = {
                                .base_type = base_type,
                                .n_size = std::get<ObjType>((*p)->obj).n_size};
                        type.n_size = std::get<ObjType>((*p)->obj).n_size;
                        lv_variant.type->obj = type;
                    }
                }

                auto lv = Attr(-1, _LV, lv_variant);
                semantic_stack.push(lv);
                break;
            }

            case R_LV_1: {
                auto e = semantic_stack.top();
                semantic_stack.pop();
                auto lv1 = semantic_stack.top();
                semantic_stack.pop();

                auto t = std::get<AttrType>(lv1.attr).type;
                auto lv_variant = AttrType{.type = nullptr};

                if (isSameType(t, string_t))
                    lv_variant.type = char_t;
                else if (t->kindType != ARRAY_TYPE_) {
                    if (t->kindType != UNIVERSAL_) {
                        error(line, "Sema error: Kind not array");
                    }
                    lv_variant.type = universal_t;
                } else {
                    lv_variant.type = std::get<ObjArray>(t->obj).el_type;
                }

                if (!isSameType(std::get<AttrType>(e.attr).type, int_t)) {
                    error(line, "Sema error: Invalid index type");
                }

                auto lv = Attr(-1, _LV, lv_variant);
                semantic_stack.push(lv);
                break;
            }

            case R_LV_2: {
                auto idu = semantic_stack.top();
                semantic_stack.pop();

                auto p = std::get<AttrId>(idu.attr).obj;
                auto lv_variant = AttrType{.type = nullptr};

                if (p->kindType != VAR_ and p->kindType != PARAM_) {
                    if (p->kindType != UNIVERSAL_) {
                        error(line, "Sema error: Kind not var");
                    }
                    lv_variant.type = universal_t;
                } else {
                    lv_variant.type = std::get<ObjType>(p->obj).type;

                    std::shared_ptr<Obj> base_type = nullptr;
                    if (std::holds_alternative<ObjAlias>(lv_variant.type->obj))
                        base_type = std::get<ObjAlias>(lv_variant.type->obj).base_type;

                    ObjAlias type = {
                            .base_type = base_type,
                            .n_size = std::get<ObjType>(p->obj).n_size};
                    lv_variant.type->obj = type;
                }

                auto lv = Attr(-1, _LV, lv_variant);
                semantic_stack.push(lv);
                break;
            }

            case R_NB_0: {
                symtable.newBlock();
                break;
            }

            case R_NF_0: {
                auto idd = semantic_stack.top();

                auto p = std::get<AttrId>(idd.attr).obj;
                auto f_obj = ObjFunc{};

                p->kindType = FUNCTION_;
                f_obj.n_params = 0;
                f_obj.ret_type = nullptr;
                f_obj.params = {};
                f_obj.n_vars = 0;
                f_obj.n_idx = num_funcs++;
                p->obj = f_obj;

                symtable.newBlock();
                break;
            }

            case R_MF_0: {
                auto t_nonterm = semantic_stack.top();
                semantic_stack.pop();
                auto lp = semantic_stack.top();
                semantic_stack.pop();
                auto idd = semantic_stack.top();
                semantic_stack.pop();

                // Shove it back into the stack
                semantic_stack.push(idd);
                semantic_stack.push(lp);
                semantic_stack.push(t_nonterm);

                auto p = std::get<AttrId>(idd.attr).obj;
                auto t = std::get<AttrType>(t_nonterm.attr).type;
                auto lp_list = std::get<AttrList>(lp.attr).list;
                auto f_obj = std::get<ObjFunc>(p->obj);

                p->kindType = FUNCTION_;
                f_obj.ret_type = t;
                f_obj.n_params = lp.size;
                f_obj.params = lp_list;
                f_obj.n_vars = 0;
                p->obj = f_obj;
                curr_func = p;
                break;
            }

            case R_MC_0: {
                auto idu = semantic_stack.top();

                auto p = std::get<AttrId>(idu.attr).obj;
                auto mc_variant =
                        AttrMc{.type = nullptr, .param = {}, .err = false};

                if (p->kindType != FUNCTION_) {
                    error(line, "Sema error: Kind not function");
                    mc_variant.type = universal_t;
                    mc_variant.param = {};
                    mc_variant.err = true;
                } else {
                    mc_variant.type = std::get<ObjFunc>(p->obj).ret_type;
                    mc_variant.param = std::get<ObjFunc>(p->obj).params;
                    mc_variant.err = false;
                }

                auto mc = Attr(-1, _MC, mc_variant);
                semantic_stack.push(mc);
                break;
            }

            case R_IDD_0: {
                int name = last_snd_token;
                Obj id(name, NO_KIND_DEF_, std::monostate());

                auto symbol = symtable.searchWhenDeclared(id);
                if (symbol.has_value()) {
                    error(line, "Sema error: Redeclared");
                } else
                    symbol = symtable.define(id);

                semantic_stack.emplace(1, _IDD, AttrId{.obj = *symbol.value(), .name = name});
                break;
            }

            case R_IDU_0: {
                int name = last_snd_token;
                Obj id(name, NO_KIND_DEF_, std::monostate());

                auto symbol = symtable.findWhenUsed(id);
                if (!symbol.has_value()) {
                    error(line, "Sema error: Not declared");
                    symbol = symtable.define(id);
                }

                semantic_stack.emplace(1, _IDU, AttrId{.obj = *symbol.value(), .name = name});
                break;
            }

            case R_TRUE_0: {
                auto true_variant = AttrBool{.type = bool_t, .val = true};
                auto true_nontem = Attr(-1, _TRUE, true_variant);
                semantic_stack.push(true_nontem);
                break;
            }

            case R_FALSE_0: {
                auto false_variant = AttrBool{.type = bool_t, .val = false};
                auto false_nontem = Attr(-1, _FALSE, false_variant);
                semantic_stack.push(false_nontem);
                break;
            }

            case R_C_0: {
                char val = std::get<char>(constants[last_const_idx]);
                auto char_variant =
                        AttrChar{.type = char_t, .pos = last_snd_token, .val = val};
                auto char_nontem = Attr(-1, _C, char_variant);
                semantic_stack.push(char_nontem);
                break;
            }

            case R_STR_0: {
                std::string val = std::get<std::string>(constants[last_const_idx]);
                auto string_variant = AttrString{
                        .type = string_t, .pos = last_snd_token, .val = val};
                auto string_nontem = Attr(-1, _STR, string_variant);
                semantic_stack.push(string_nontem);
                break;
            }

            case R_NUM_0: {
                int val = std::get<int>(constants[last_const_idx]);
                auto num_variant =
                        AttrNum{.type = int_t, .pos = last_snd_token, .val = val};
                auto num_nontem = Attr(-1, _NUM, num_variant);
                semantic_stack.push(num_nontem);
                break;
            }
        }
    }
}