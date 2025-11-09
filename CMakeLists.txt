cmake_minimum_required(VERSION 3.15)

project(simple_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(SDL_FRAMEWORK_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3")
set(CMAKE_FRAMEWORK_PATH "${SDL_FRAMEWORK_DIR}" ${CMAKE_FRAMEWORK_PATH})

find_library(SDL3_FRAMEWORK SDL3
    REQUIRED
    PATHS "${SDL_FRAMEWORK_DIR}"
    NO_DEFAULT_PATH
)

find_library(SDL3_IMAGE_FRAMEWORK SDL3_image
    REQUIRED
    PATHS "${SDL_FRAMEWORK_DIR}"
    NO_DEFAULT_PATH
)

set(GAME_ENGINE_DIR "${CMAKE_SOURCE_DIR}/2D_GameEngine")

add_executable(simple_game
    "${GAME_ENGINE_DIR}/main.cpp"
    "${GAME_ENGINE_DIR}/Game.cpp"
)

target_include_directories(simple_game PRIVATE
    "${GAME_ENGINE_DIR}"
    "${SDL3_FRAMEWORK}/Headers"
    "${SDL3_IMAGE_FRAMEWORK}/Headers"
)
target_link_libraries(simple_game PRIVATE
    "${SDL3_FRAMEWORK}"
    "${SDL3_IMAGE_FRAMEWORK}"
)
