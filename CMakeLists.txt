cmake_minimum_required(VERSION 3.2)
project(compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp CppGenerator.cpp CppGenerator.h TokenProcessor.cpp TokenProcessor.h utils.h QuotedWord.h asm_exception.h)
add_executable(compiler ${SOURCE_FILES})


#file(GLOB ExtraFiles ${CMAKE_SOURCE_DIR}/extra/*)
#add_custom_target(copy)
#foreach(ExtraFile ${ExtraFiles})
#  add_custom_command(TARGET copy PRE_BUILD
#                     COMMAND ${CMAKE_COMMAND} -E
#                         copy ${ExtraFile} $<TARGET_FILE_DIR:MyTarget>)
#endforeach()

configure_file(extra/main_ex.cpp main_ex.cpp COPYONLY)
configure_file(extra/teste.lgo teste.lgo COPYONLY)
