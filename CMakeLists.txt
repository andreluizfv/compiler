cmake_minimum_required(VERSION 3.24)
project(compiler)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(compiler
        constants.h
        main.cpp
        searchKeyWord.h
        t_const.h
        t_token.h)
