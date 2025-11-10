cmake_minimum_required(VERSION 3.20)

project(simple_game LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(Options)
include(CompilerFlags)
include(Dependencies)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GAME_ENGINE_DIR "${CMAKE_SOURCE_DIR}/2D_GameEngine")

add_executable(simple_game
    "${GAME_ENGINE_DIR}/main.cpp"
    "${GAME_ENGINE_DIR}/Game.cpp"
)

target_include_directories(simple_game PRIVATE "${GAME_ENGINE_DIR}")

setup_sdl_dependencies(simple_game)
set_compiler_flags(TARGET_NAMES simple_game)

install(TARGETS simple_game RUNTIME DESTINATION "${DESTINATION_DIR}")
