include(FetchContent)

message(STATUS "Fetching GLAD...")

find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Force GLAD to use the same Python interpreter as CMake
# GLAD 2 internal CMake uses find_package(Python) and ${Python_EXECUTABLE}
set(Python_EXECUTABLE "${Python3_EXECUTABLE}" CACHE FILEPATH "Python interpreter for GLAD" FORCE)
set(PYTHON_EXECUTABLE "${Python3_EXECUTABLE}" CACHE FILEPATH "Python interpreter for GLAD" FORCE)
set(GLAD_PYTHON_EXECUTABLE "${Python3_EXECUTABLE}" CACHE FILEPATH "Python interpreter for GLAD" FORCE)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v2.0.8
    GIT_PROGRESS   TRUE
    SOURCE_SUBDIR  cmake
)

FetchContent_MakeAvailable(glad)
