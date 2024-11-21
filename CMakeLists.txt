cmake_minimum_required(VERSION 3.28)
project(Game)

set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(Game
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
        overlay.h
)

# Set SFML directory
set(SFML_DIR C:/SFML/lib/cmake/SFML)

# Include SFML headers
include_directories(C:/SFML/include/SFML)
# Set SFML static libraries
set(SFML_STATIC_LIBRARIES TRUE)

# Find SFML components for static linking
find_package(SFML COMPONENTS graphics window system audio network REQUIRED)

# Link SFML static libraries
target_link_libraries(Game sfml-graphics sfml-window sfml-system sfml-audio sfml-network)

# Set static linking flags
set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
