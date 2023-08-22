#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <limits.h>
#include "t_const.h"
#include <vector>
#include <exception>
#include <string>
using namespace std;

const int MAX_CONSTS = 100;

class NotYetDeclaredConstantException : public std::exception {
public:
    NotYetDeclaredConstantException() : errorMessage("the current index belongs to a literal not yet used") {}
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
private:
    std::string errorMessage;
};

class WrongConstTypeException : public std::exception {
public:
    WrongConstTypeException() : errorMessage("the constat at this index is not of the desired type") {}
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
private:
    std::string errorMessage;
};

class Constants{
private:
    static t_const _constants[MAX_CONSTS];
    static int nextConstantIndex;
public:
    static int addCharConst(char c){
        _constants[nextConstantIndex].type = CHAR_CONST;
        _constants[nextConstantIndex].cVal = c;
        return nextConstantIndex++;
    }
    static int addIntConst(int n){
        _constants[nextConstantIndex].type = INT_CONST;
        _constants[nextConstantIndex].nVal = n;
        return nextConstantIndex++;
    }
    static int addStringConst(char* s){
        _constants[nextConstantIndex].type = STRING_CONST;
        int size = strlen(s);
        _constants[nextConstantIndex].sVal = (char*) malloc((size + 1) * sizeof(char));
        strncpy(_constants[nextConstantIndex].sVal, s,size);
        _constants[nextConstantIndex].sVal[size] = '\0';
        return nextConstantIndex++;
    }
    static char getCharConst(int n){
        if(n >= nextConstantIndex) throw NotYetDeclaredConstantException();
        if(_constants[n].type != CHAR_CONST) throw WrongConstTypeException();
        return _constants[n].cVal;
    }
    static char* getStringConst(int n){
        if(n >= nextConstantIndex) throw NotYetDeclaredConstantException();
        if(_constants[n].type != STRING_CONST) throw WrongConstTypeException();
        return _constants[n].sVal;
    }
    static int getIntConst(int n){
        if(n >= nextConstantIndex) throw NotYetDeclaredConstantException();
        if(_constants[n].type != INT_CONST) throw WrongConstTypeException();
        return _constants[n].nVal;
    }
};
int Constants::nextConstantIndex = 0;
t_const Constants::_constants[] = {}; 
#endif // T_TOKEN_H