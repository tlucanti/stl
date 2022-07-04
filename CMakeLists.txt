
cmake_minimum_required(VERSION 3.21)
project(stl)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-O0 -Wall -Wextra -I inc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wshadow=compatible-local -Wcast-qual -Wconversion -Wzero-as-null-pointer-constant -Wextra-semi -Wsign-conversion -Wlogical-op")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

include_directories(inc)

add_executable(stl test/test_vector.cpp)
