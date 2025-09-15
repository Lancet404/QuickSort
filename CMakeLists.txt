cmake_minimum_required(VERSION 3.30)
project(QuickSort)

set(CMAKE_CXX_STANDARD 20)

add_executable(QuickSort main.cpp
        sort.cpp
        sort_test.cpp
)
