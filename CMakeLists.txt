cmake_minimum_required(VERSION 3.2)
project(compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    AsmGenerator.cpp
    AsmGenerator.h
    main.cpp
    QuotedWord.h
    TokenProcessor.cpp
    TokenProcessor.h)

add_executable(compiler ${SOURCE_FILES})

add_custom_command(TARGET compiler POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/main_ex.cpp"      # <--this is in-file
        $<TARGET_FILE_DIR:compiler>)                 # <--this is out-file path

add_custom_command(TARGET compiler POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/teste.lgo"      # <--this is in-file
        $<TARGET_FILE_DIR:compiler>)                 # <--this is out-file path