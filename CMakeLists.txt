cmake_minimum_required(VERSION 3.15)
project(learn-sfml
    LANGUAGES CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(thirdparty)
add_subdirectory(src)
