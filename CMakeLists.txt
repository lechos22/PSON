cmake_minimum_required(VERSION 3.20)
project(PSON)

set(CMAKE_CXX_STANDARD 20)


if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    add_compile_options(-g -Os -Wall -Wextra -Werror)
    add_link_options(-g -Os -Wall -Wextra -Werror)
    if(WIN32)
        add_link_options(-static-libgcc -static-libstdc++)
    endif()
    message(STATUS "Using GNU compiler")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "CLANG")
    add_compile_options(-g -Os -Wall -Wextra -Werror)
    add_link_options(-g -Os -Wall -Wextra -Werror)
    message(STATUS "Using clang compiler")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    add_compile_options(/Os /WX /MP)
    add_link_options(/WX)
    message(STATUS "Using MSVC compiler")
endif()

add_library(PSON STATIC PSON.hpp PSON.cpp parser.hpp parser.cpp lexer.hpp lexer.cpp)

add_executable(test.lexer${CMAKE_EXECUTABLE_SUFFIX} tests/test.lexer.cpp PSON)
target_link_libraries(test.lexer${CMAKE_EXECUTABLE_SUFFIX} PSON)

enable_testing()

add_test(NAME lexer COMMAND test.lexer)
