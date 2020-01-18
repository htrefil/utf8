cmake_minimum_required(VERSION 3.10)

project(utf8 C)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)

add_library(utf8 ${SOURCES})