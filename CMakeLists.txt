cmake_minimum_required(VERSION 3.20)
project(PSON)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

file(COPY examples DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library(libPSON STATIC libpson.hpp libpson.cpp)

add_executable(test.libPSON
        libpson.hpp test.libpson.cpp)
target_link_libraries(test.libPSON libPSON)

enable_testing
add_test(test.libPSON COMMAND test.libPSON)
