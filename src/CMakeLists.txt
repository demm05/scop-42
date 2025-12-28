file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "*.cpp")

glad_add_library(glad_gl REPRODUCIBLE API gl:core=4.6)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw glad_gl
                                              spdlog::spdlog m)

target_include_directories(
  ${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include"
                          "${CMAKE_CURRENT_SOURCE_DIR}")

# --- Future Modularity Hint ---
# When you want to modularize later (e.g., making a 'math' library), you would
# create a src/math/CMakeLists.txt with: add_library(my_math STATIC matrix.cpp
# vector.cpp) And then here in src/CMakeLists.txt, you would change:
# target_link_libraries(scop PRIVATE my_math)
