cmake_minimum_required(VERSION 3.28)
project(MissileGame)

set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(MissileGame
        main.cpp
        stateManager.cpp
        screen.cpp
        menu.cpp
        button.cpp
        missile.cpp
        plane.cpp
        math.cpp
        overlay.cpp
)

# SFML configuration
# Use the SFML_DIR environment variable passed by the GitHub Actions workflow
if(DEFINED ENV{SFML_DIR})
    set(SFML_DIR $ENV{SFML_DIR})
else ()
    set(SFML_DIR C:/SFML/lib/cmake/SFML)
endif()

# Look for SFML package
find_package(SFML REQUIRED COMPONENTS graphics window system audio network)

# Include SFML headers
target_include_directories(MissileGame PRIVATE ${SFML_INCLUDE_DIR})

# Link SFML libraries
target_link_libraries(MissileGame PRIVATE sfml-graphics sfml-window sfml-system sfml-audio sfml-network)

# Platform-specific definitions
if(WIN32)
    add_definitions(-D_WINDOWS)
elseif(UNIX)
    add_definitions(-D_LINUX)
endif()

# Compiler-specific flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(MissileGame PRIVATE -Wall -Wextra -pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(MissileGame PRIVATE /W4)
endif()
