cmake_minimum_required(VERSION 3.30)
project(qs_sort CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем тестирование
enable_testing()

# GoogleTest через FetchContent
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(qs_sort INTERFACE)
target_include_directories(qs_sort INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(sort_test sort_test.cpp)
target_link_libraries(sort_test PRIVATE qs_sort GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(sort_test)
