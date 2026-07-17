cmake_minimum_required(VERSION 3.31)
project(ParseLi VERSION 0.0.4)

add_library(ParseLi parselib.cpp)
target_include_directories(ParseLi INTERFACE ${CMAKE_CURRENT_LIST_DIR})
