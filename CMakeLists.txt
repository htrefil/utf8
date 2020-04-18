cmake_minimum_required(VERSION 3.10)

project(utf8 C)

file(GLOB_RECURSE SOURCES src/*.c)

add_library(utf8 ${SOURCES})

target_include_directories(utf8 PUBLIC include)
target_include_directories(utf8 PRIVATE include/utf8)