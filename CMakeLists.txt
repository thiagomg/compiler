cmake_minimum_required(VERSION 3.2)
project(compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp CppGenerator.cpp CppGenerator.h TokenProcessor.cpp TokenProcessor.h utils.h QuotedWord.h asm_exception.h)
add_executable(compiler ${SOURCE_FILES})