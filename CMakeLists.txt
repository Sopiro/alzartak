cmake_minimum_required(VERSION 3.27)

project(alzartak LANGUAGES C CXX VERSION 0.0.1)

option(ALZARTAK_BUILD_SAMPLE "Build sample project" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

add_subdirectory(extern)
add_subdirectory(src)

if(ALZARTAK_BUILD_SAMPLE)
    message("Build Alzartak sample")
    add_subdirectory(sample)
endif()