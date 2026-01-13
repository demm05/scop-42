cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME scop)

project(
  ${PROJECT_NAME}
  VERSION 1.0.0
  LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Fix for library discovery on Ubuntu/Debian systems
if(UNIX AND NOT APPLE)
  list(APPEND CMAKE_LIBRARY_PATH /usr/lib/x86_64-linux-gnu)
  list(APPEND CMAKE_INCLUDE_PATH /usr/include/x86_64-linux-gnu /usr/include)
  
  # Explicitly add these for the compiler if it's isolated (like in pixi/conda)
  add_compile_options("-I/usr/include" "-I/usr/include/x86_64-linux-gnu")
  add_link_options("-L/usr/lib/x86_64-linux-gnu")
  add_link_options("-Wl,-rpath-link,/usr/lib/x86_64-linux-gnu")
  add_link_options("-static-libstdc++" "-static-libgcc")
  link_directories(/usr/lib/x86_64-linux-gnu)
endif()

include("cmake/LinkGLFW.cmake")
include("cmake/LinkGLAD.cmake")
include("cmake/LinkLinMath.cmake")
include("cmake/LinkSpdlog.cmake")

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wformat=2
    -Wconversion
    -Wunused
    -Werror=return-type)

  # Development mode flags (Sanitizers)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE)
    message(STATUS "Enabling Sanitizers")
    set(SAN_FLAGS -fsanitize=address,undefined -fno-omit-frame-pointer -g3)
    add_compile_options(${SAN_FLAGS} -D_GLIBCXX_ASSERTIONS -D_GLIBCXX_DEBUG)
    add_link_options(${SAN_FLAGS})
  endif()
endif()

add_subdirectory(src)
