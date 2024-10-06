cmake_minimum_required(VERSION 3.22)

cmake_policy(SET CMP0048 NEW)
project(QTRADR VERSION 0.0.0)

set(CMAKE_C_COMPILER "/usr/bin/gcc-13" CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-13" CACHE PATH "" FORCE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(deps/FindGTest.cmake)
include(deps/FindBenchmark.cmake)
include(deps/FindTOMLplusplus.cmake)
include(deps/FindOpenSSL.cmake)

enable_testing()

include(cmake/warnings.cmake)

add_subdirectory(dsa/)
add_subdirectory(network/)
add_subdirectory(concurrency/)
add_subdirectory(multiplexing/)
add_subdirectory(memory/)
add_subdirectory(misc/)
add_subdirectory(timer/)
