cmake_minimum_required(VERSION 3.22)
project(file_reading_analysis)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER clang++)

include_directories(methods)

set(
        analysis_source
        ${file_reading_analysis_SOURCE_DIR}/methods/methods.cpp
        ${file_reading_analysis_SOURCE_DIR}/methods/methods.h
)

add_executable(file_reading_analysis
        ${analysis_source}
        main.cpp)

add_subdirectory(tests)