include(FetchContent)

# 1. GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)

# Force GLFW to see the system headers in this environment
if(UNIX AND NOT APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/include -I/usr/include/x86_64-linux-gnu" CACHE STRING "" FORCE)
endif()

# 2. spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.16.0
)
set(SPDLOG_BUILD_ALL OFF CACHE BOOL "" FORCE)

# 3. linmath.h
FetchContent_Declare(
    linmath
    GIT_REPOSITORY https://github.com/datenwolf/linmath.h.git
    GIT_TAG        master
)

# 4. GLAD
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v2.0.8
    SOURCE_SUBDIR  cmake
)

# Apply all
FetchContent_MakeAvailable(glfw spdlog linmath glad)

# Wrap linmath
if(NOT TARGET linmath)
    add_library(linmath INTERFACE)
    target_include_directories(linmath INTERFACE ${linmath_SOURCE_DIR})
endif()
