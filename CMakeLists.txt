cmake_minimum_required(VERSION 3.20)
project(PSON)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SWIG_DIR $ENV{SWIG_DIR})
set(SWIG_LIB $ENV{SWIG_LIB})

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    add_compile_options(-g -Os)
    add_link_options(-g -Os)
    if(WIN32)
        add_link_options(-static-libgcc -static-libstdc++)
    endif()
    message(STATUS "Using GNU compiler")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "CLANG")
    add_compile_options(-g -Os)
    add_link_options(-g -Os)
    message(STATUS "Using clang compiler")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    add_compile_options(/Os /WX /MP)
    add_link_options(/WX)
    message(STATUS "Using MSVC compiler")
endif()

find_package(SWIG REQUIRED)
include(UseSWIG)
find_package(Java COMPONENTS Development REQUIRED)
find_package(Python3 COMPONENTS Development REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

include_directories(${JAVA_INCLUDE_PATH})
include_directories(${JAVA_INCLUDE_PATH2})
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${SWIG_LIB})
include_directories(${SWIG_LIB}/java)
include_directories(${SWIG_LIB}/python)
include_directories(.)
link_directories(${JAVA_JVM_PATH})
link_directories(${Python3_LIBRARY_DIRS})

add_library(PSON STATIC PSON.hpp PSON.cpp parser.hpp parser.cpp lexer.hpp lexer.cpp symbol.hpp symbol.cpp foreign.cpp foreign.hpp)

add_executable(test.lexer tests/test.lexer.cpp PSON)
target_link_libraries(test.lexer PSON)

add_executable(test.parser tests/test.parser.cpp PSON)
target_link_libraries(test.parser PSON)

set_property(SOURCE PSON.i PROPERTY CPLUSPLUS ON)

swig_add_library(PSONJNI TYPE SHARED LANGUAGE java SOURCES PSON.i)
swig_link_libraries(PSONJNI PSON)

swig_add_library(PSON_PY TYPE SHARED LANGUAGE python SOURCES PSON.i)
swig_link_libraries(PSON_PY PSON)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/PSON_Wrapper.java ${CMAKE_CURRENT_BINARY_DIR}/PSON_Wrapper.java COPYONLY)

install(TARGETS PSON PSONJNI PSON_PY DESTINATION lib)

enable_testing()

add_test(NAME lexer COMMAND test.lexer${CMAKE_EXECUTABLE_SUFFIX})
add_test(NAME parser COMMAND test.parser${CMAKE_EXECUTABLE_SUFFIX})
