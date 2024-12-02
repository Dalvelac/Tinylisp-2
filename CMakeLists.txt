cmake_minimum_required(VERSION 3.29)
project(Tinylisp_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Tinylisp_2
        main.cpp
        src/calculator.cpp
        src/calculator.h)
