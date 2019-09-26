cmake_minimum_required(VERSION 3.14)
project(a1)

set(CMAKE_CXX_STANDARD 14)

add_executable(a1 dragonshell.cc builtin.cc builtin.h)