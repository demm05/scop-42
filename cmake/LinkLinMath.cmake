include(FetchContent)

FetchContent_Declare(
  linmath
  GIT_REPOSITORY https://github.com/datenwolf/linmath.h.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(linmath)

add_library(linmath INTERFACE)
target_include_directories(linmath INTERFACE ${linmath_SOURCE_DIR})
