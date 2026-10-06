cmake_minimum_required(VERSION 3.20)
project(Lab2 CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(Main main.cpp)
target_link_libraries(Main pthread dl)

add_library(mydll SHARED mydll.cpp)