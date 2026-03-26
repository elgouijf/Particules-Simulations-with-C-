cmake_minimum_required(VERSION 3.16.3)

project(TP4 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_compile_definitions(PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)
add_subdirectory(demo)
