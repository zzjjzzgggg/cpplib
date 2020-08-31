cmake_minimum_required(VERSION 3.10.1)
project (cpplib C CXX)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX_17)
check_cxx_compiler_flag("-std=c++14" COMPILER_SUPPORTS_CXX_14)

if (COMPILER_SUPPORTS_CXX_17)
  message(STATUS "Compiling with C++17")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
elseif (COMPILER_SUPPORTS_CXX_14)
  message(STATUS "Compiling with C++14")
  set(CMAKE_CXX_STANDARD 14)
else()
  message(FATAL_ERROR "Please install a modern C++ compiler.")
endif()

set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(gflags REQUIRED)

include_directories(lz4/lib)
add_library(lz4 SHARED lz4/lib/lz4.c)
set_target_properties(lz4 PROPERTIES COMPILE_FLAGS "-c -x c")

include_directories(Assert/src)
add_library(ppk_assert SHARED Assert/src/ppk_assert.cpp)

include_directories(str)
include_directories(os)
include_directories(io)
include_directories(graph)
include_directories(math)
include_directories(adv)

add_subdirectory(str)
add_subdirectory(os)
add_subdirectory(io)
add_subdirectory(graph)
add_subdirectory(math)
add_subdirectory(adv)

add_subdirectory(test)
