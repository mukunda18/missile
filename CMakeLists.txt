cmake_minimum_required(VERSION 3.16)
project(MissileGame)

set(CMAKE_CXX_STANDARD 17)
set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML COMPONENTS graphics audio REQUIRED)

add_executable(MissileGame
        main.cpp
        button.h
        button.cpp
        StateManager.h
        StateManager.cpp
        screen.h
        screen.cpp
        menu.h
        menu.cpp
        overlay.h
        overlay.cpp
        plane.h
        plane.cpp
        missile.h
        missile.cpp
        ability.cpp
        ability.h
        bar.cpp
        bar.h
)

target_link_libraries(MissileGame sfml-graphics sfml-audio)

