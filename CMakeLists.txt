cmake_minimum_required(VERSION 3.17)
project(CommoTalkie)
set(CMAKE_CXX_STANDARD 20)

# Check for CppUTest and bail out if they don't have it
if(DEFINED ENV{CPPUTEST_HOME} AND DEFINED ENV{CPPUTESTEXT_HOME} AND DEFINED ENV{CPPUTEST_PARENT_DIR})
    message("Using CppUTest found in $ENV{CPPUTEST_HOME}")
    message("Using CppUTest Extensions found in $ENV{CPPUTESTEXT_HOME}")
    message("Using CppUTest library basedir found in $ENV{CPPUTEST_PARENT_DIR}")
else()
    message("CPPUTEST_HOME or CPPUTESTEXT_HOME or CPPUTEST_PARENT_DIR is not set; You must tell CMake where to find CppUTest")
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

# Dependencies

# FIXME Dependency management is a bit messy. May be trying with conan.io?

file(DOWNLOAD
        https://raw.githubusercontent.com/westial/powermanagementunit/main/include/sleepable/Sleepable.h
        ${CMAKE_SOURCE_DIR}/include/sleepable/Sleepable.h)

file(DOWNLOAD
        https://raw.githubusercontent.com/westial/powermanagementunit/main/include/sleepable/SleepablePrivate.h
        ${CMAKE_SOURCE_DIR}/include/sleepable/SleepablePrivate.h)