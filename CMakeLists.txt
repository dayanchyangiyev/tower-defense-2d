cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "tower-defense-2d")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME} LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(Options)
include(CompilerFlags)
include(Dependencies)
include(CopyHelper)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GAME_ENGINE_DIR "${CMAKE_SOURCE_DIR}/2D_GameEngine")
set(SRC_DIR "${GAME_ENGINE_DIR}/src")
set(HEADERS_DIR "${GAME_ENGINE_DIR}/headers")

add_executable(${MAIN_EXECUTABLE_NAME}
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/Game.cpp"
    "${SRC_DIR}/TextureManager.cpp"
    "${SRC_DIR}/GameObject.cpp"
    "${SRC_DIR}/Level.cpp"
    "${SRC_DIR}/Map.cpp"
    "${SRC_DIR}/Logger.cpp"
)

target_include_directories(${MAIN_EXECUTABLE_NAME} PRIVATE "${HEADERS_DIR}")

setup_sdl_dependencies(${MAIN_EXECUTABLE_NAME})
set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

install(TARGETS ${MAIN_EXECUTABLE_NAME} RUNTIME DESTINATION "${DESTINATION_DIR}")
if(APPLE)
    install(FILES launcher.command DESTINATION "${DESTINATION_DIR}")
endif()

copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
