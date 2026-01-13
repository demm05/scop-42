if(UNIX AND NOT APPLE)
    # Fix for library discovery on Ubuntu/Debian systems when using isolated compilers (Pixi/Conda)
    message(STATUS "Applying Linux Platform Fixes...")

    # We use CMAKE_LIBRARY_PATH so find_package(OpenGL) and find_package(X11) work
    list(APPEND CMAKE_LIBRARY_PATH "/usr/lib/x86_64-linux-gnu")
    list(APPEND CMAKE_INCLUDE_PATH "/usr/include/x86_64-linux-gnu" "/usr/include")

    # Adding system paths as SYSTEM tells the compiler to ignore warnings from these headers
    include_directories(SYSTEM "/usr/include" "/usr/include/x86_64-linux-gnu")

    # Linker hacks to find transitive dependencies
    add_link_options("-L/usr/lib/x86_64-linux-gnu")
    add_link_options("-Wl,-rpath-link,/usr/lib/x86_64-linux-gnu")

    # Static linking for libstdc++ to prevent version mismatch on older systems
    add_link_options("-static-libstdc++" "-static-libgcc")
endif()
