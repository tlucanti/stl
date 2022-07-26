
cmake_minimum_required(VERSION 3.21)
project(stl)

set(CMAKE_CXX_STANDARD 98)
if (LINUX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wshadow=compatible-local -Wcast-qual -Wconversion -Wzero-as-null-pointer-constant -Wextra-semi -Wsign-conversion -Wlogical-op")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Wfloat-equal -Wshadow -Wcast-qual -Wconversion -Wzero-as-null-pointer-constant -Wextra-semi -Wsign-conversion")
endif(LINUX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__DEBUG")

include_directories(inc)
include_directories(test)
include_directories(rb_tree)
include_directories(pair)
include_directories(iterator)

add_executable(vector_test vector/test_vector.cpp)
add_executable(stack_test stack/test_stack.cpp)
add_executable(pair_test pair/test_pair.cpp)
add_executable(map_test map/test_map.cpp)
add_executable(set_test set/test_set.cpp)
add_executable(rb_tree_test rb_tree/test_rb_tree.cpp rb_tree/rb_tree.c)

add_executable(vector_speed vector/speed_test.cpp)

add_executable(test test/main.cpp)