# Source Discovery
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "*.cpp")

# GLAD Generation
glad_add_library(glad_gl REPRODUCIBLE API gl:core=4.6)

# Binary Target
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Apply strict flags and sanitizers only to OUR target
target_compile_options(${PROJECT_NAME} PRIVATE ${PROJECT_WARNINGS} ${PROJECT_DEBUG_COMPILE_FLAGS})
target_link_options(${PROJECT_NAME} PRIVATE ${PROJECT_LINKER_FLAGS})

# Linking
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        OpenGL::OpenGL
        glfw
        glad_gl
        spdlog::spdlog
        m
)

# Include Paths
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}"
    SYSTEM PRIVATE
        "/usr/include"
        "/usr/include/x86_64-linux-gnu"
)
