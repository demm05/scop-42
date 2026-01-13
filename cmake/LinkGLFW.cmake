include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

if(UNIX AND NOT APPLE)
    # Automatic detection of backend dependencies
    include(FindPkgConfig)
    pkg_check_modules(WAYLAND QUIET wayland-client wayland-protocols libxkbcommon)
    find_package(X11 QUIET)

    # Determine default values based on what's found on the system
    if(WAYLAND_FOUND)
        set(GLFW_BUILD_WAYLAND_DEFAULT ON)
    else()
        set(GLFW_BUILD_WAYLAND_DEFAULT OFF)
    endif()

    if(X11_FOUND)
        set(GLFW_BUILD_X11_DEFAULT ON)
    else()
        set(GLFW_BUILD_X11_DEFAULT OFF)
    endif()

    # Define options with the detected defaults. If the user already specified 
    # these on the command line, those values will take precedence.
    option(GLFW_BUILD_WAYLAND "Build Wayland support" ${GLFW_BUILD_WAYLAND_DEFAULT})
    option(GLFW_BUILD_X11 "Build X11 support" ${GLFW_BUILD_X11_DEFAULT})

    # Log the configuration for better visibility
    message(STATUS "GLFW Backend Configuration:")
    message(STATUS "  - Wayland: ${GLFW_BUILD_WAYLAND} (Detected: ${WAYLAND_FOUND})")
    message(STATUS "  - X11:     ${GLFW_BUILD_X11} (Detected: ${X11_FOUND})")
endif()

FetchContent_MakeAvailable(glfw)
