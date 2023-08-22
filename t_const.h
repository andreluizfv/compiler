#pragma once
#ifndef T_CONST_H
#define T_CONST_H

enum type_t_const{
    CHAR_CONST, INT_CONST, STRING_CONST
};

struct t_const {
    enum type_t_const type;
    union{
        char cVal;
        int nVal;
        char* sVal;
    };
};
typedef struct t_const t_const;
#endif // T_TOKEN_H