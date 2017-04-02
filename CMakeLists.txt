cmake_minimum_required(VERSION 3.1)

project(testUrllib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11")

set(CMAKE_CXX_COMPLIER "clang++")

aux_source_directory(. DIR_SRCS)

add_executable(testUrllib ${DIR_SRCS})

target_link_libraries(testUrllib ssl crypto)
