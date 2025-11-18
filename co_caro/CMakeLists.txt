cmake_minimum_required(VERSION 3.22)
project(CaroSFML)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "D:/BTL_C++_PTIT/SFML-3.0.2/install/lib/cmake/SFML")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio Network)

add_executable(caro
    src/main.cpp
    src/game.cpp
    src/board.cpp
    src/player.cpp
    src/display.cpp
    src/input.cpp
    src/file_manager.cpp
    src/event_input.cpp
    src/menu.cpp
    src/sound.cpp
    src/music_game.cpp
    src/human_player.cpp
    src/AIplayer.cpp
    src/gadget.cpp
    resource.rc
)

target_link_libraries(caro PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)
