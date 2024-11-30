cmake_minimum_required(VERSION 3.28)
project(MissileGame)

set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(MissileGame
        main.cpp
        stateManager.cpp
        stateManager.h
        screen.cpp
        screen.h
        menu.cpp
        menu.h
        button.cpp
        button.h
        missile.cpp
        missile.h
        plane.cpp
        plane.h
        math.cpp
        overlay.cpp
        overlay.h
)

# SFML configuration
set(SFML_DIR C:/SFML/lib/cmake/SFML)

# Include SFML headers
include_directories(C:/SFML/include/SFML)


find_package(SFML COMPONENTS graphics window system audio network REQUIRED)

# Link static SFML libraries
target_link_libraries(MissileGame
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
        sfml-main
)
