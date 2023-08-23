#pragma once
#include <string.h>
#include <iostream>
#include "t_token.h"
#ifndef SEARCH_KEY_WORD_H
#define SEARCH_KEY_WORD_H
char* reservedKeywords[] = {
    "ARRAY", "BOOLEAN", "BREAK", "CHAR", "CONTINUE", "DO", "ELSE", "FALSE", "FUNCTION", "IF",
    "INTEGER", "OF", "STRING", "STRUCT", "TRUE", "TYPE", "VAR", "WHILE"};

int searchKeyWord(char* word){
    int left = 0;
    int right = sizeof(reservedKeywords)/sizeof(char*) - 1;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (strcmp(reservedKeywords[mid], word) == 0) {
            return mid; 
        } else if (strcmp(reservedKeywords[mid],word) < 0) {
            left = mid + 1; 
        } else {
            right = mid - 1;
        }
    }
    return UNKNOWN;
}


#endif