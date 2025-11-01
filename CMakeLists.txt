cmake_minimum_required(VERSION 3.14)
project(lin_alg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# library target
add_library(lin_alg INTERFACE)
target_include_directories(lin_alg INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
add_subdirectory(tests)
