cmake_minimum_required(VERSION 3.12)

project(WebBench LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_FLAGS "-pedantic -Wall -O3 -g")

add_executable(bench src/bench.cpp)
target_include_directories(bench PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(bench pthread atomic)