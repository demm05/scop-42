cmake_minimum_required(VERSION 3.22)

project(scop
    VERSION 1.0.0
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(PlatformFixes)
include(CompilerOptions)
include(Dependencies)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

add_subdirectory(src)
