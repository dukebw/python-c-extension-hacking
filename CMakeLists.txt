cmake_minimum_required(VERSION 3.24)
project(CPythonHacking VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python3 for Python module development
find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)

Python3_add_library(fib src/fib.c)
