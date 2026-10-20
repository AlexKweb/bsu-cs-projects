cmake_minimum_required(VERSION 3.20)

project(Lab3 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(Lab3 main.cpp)
target_link_libraries(Lab3 PRIVATE Threads::Threads)

enable_testing()

add_executable(Lab3Tests tests.cpp)
add_test(NAME Lab3Tests COMMAND Lab3Tests)
