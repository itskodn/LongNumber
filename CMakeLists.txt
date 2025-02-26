cmake_minimum_required(VERSION 3.19)
project(LongMath)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(hm)
add_subdirectory(hm)

add_executable(pi pi.cpp)
target_link_libraries(pi hm)

add_executable(test test.cpp)
target_link_libraries(test hm)

