cmake_minimum_required(VERSION 3.10)
project(framework)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES framework.hpp)
add_library(framework ${SOURCE_FILES})
