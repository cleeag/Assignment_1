cmake_minimum_required(VERSION 3.15)
project(Assignment_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Assignment_1
        intermediate.h
        main.cpp
        table.h table.cpp intermediate.cpp)
