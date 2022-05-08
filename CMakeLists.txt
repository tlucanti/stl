
cmake_minimum_required(VERSION 3.21)
project(stl)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-O0 -Wall -Wextra -pedantic-errors")

add_executable(stl test.cpp)
