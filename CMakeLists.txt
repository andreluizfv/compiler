cmake_minimum_required(VERSION 3.22)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(compiler
        main.cpp
        TokenType.h Utils.h Utils.cpp Token.cpp Token.h Scanner.cpp Scanner.h Syntactic.cpp)
