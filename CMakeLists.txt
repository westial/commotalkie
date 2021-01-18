cmake_minimum_required(VERSION 3.17)
project(CommoTalkie)
set(CMAKE_CXX_STANDARD 20)

# Check for CppUTest and bail out if they don't have it
if(DEFINED ENV{CPPUTEST_HOME})
    message("Using CppUTest found in $ENV{CPPUTEST_HOME}")
else()
    message("CPPUTEST_HOME is not set; You must tell CMake where to find CppUTest")
    return()
endif()

enable_language(C)
enable_language(CXX)

# The version number
set (CommoTalkie_VERSION_MAJOR 0)
set (CommoTalkie_VERSION_MINOR 1)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(mock)