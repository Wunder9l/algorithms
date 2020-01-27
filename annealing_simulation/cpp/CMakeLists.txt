cmake_minimum_required(VERSION 3.8)
project(cpp)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp annealing.h annealing.cpp)
add_executable(cpp ${SOURCE_FILES})