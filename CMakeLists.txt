
cmake_minimum_required(VERSION 3.21)
project(stl)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-O0 -Wall -Wextra -I inc")

include_directories(inc)

add_executable(stl test/test_vector.cpp)
