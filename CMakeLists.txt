cmake_minimum_required(VERSION 3.16)
project(skola)

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_CXX_FLAGS -pthread)

add_executable(skola2 main.cpp)