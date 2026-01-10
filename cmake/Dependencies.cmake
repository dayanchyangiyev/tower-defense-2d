include_guard(GLOBAL)
include(FetchContent)

function(setup_sdl_dependencies target_name)
    message(STATUS "Setting up SDL3 dependencies (Local/System)...")
    if(APPLE)
        set(_sdl_root "${CMAKE_SOURCE_DIR}/lib/SDL3")
        set(_sdl_config_dir "${CMAKE_SOURCE_DIR}/cmake")

        if(EXISTS "${_sdl_root}/SDL3.framework/SDL3")
            if(NOT DEFINED SDL3_ROOT_DIR)
                set(SDL3_ROOT_DIR "${_sdl_root}" CACHE PATH "Location of bundled SDL3.framework" FORCE)
            endif()
            if(NOT DEFINED SDL3_DIR)
                set(SDL3_DIR "${_sdl_config_dir}" CACHE PATH "Location of SDL3Config.cmake" FORCE)
            endif()
        endif()

        if(EXISTS "${_sdl_root}/SDL3_image.framework/SDL3_image")
            if(NOT DEFINED SDL3_IMAGE_ROOT_DIR)
                set(SDL3_IMAGE_ROOT_DIR "${_sdl_root}" CACHE PATH "Location of bundled SDL3_image.framework" FORCE)
            endif()
            if(NOT DEFINED SDL3_image_DIR)
                set(SDL3_image_DIR "${_sdl_config_dir}" CACHE PATH "Location of SDL3_imageConfig.cmake" FORCE)
            endif()
        endif()

        find_package(SDL3 CONFIG REQUIRED)
        find_package(SDL3_image CONFIG REQUIRED)
    else()
        find_package(SDL3 CONFIG QUIET)
        find_package(SDL3_image CONFIG QUIET)

        if(NOT SDL3_FOUND OR NOT SDL3_image_FOUND)
            message(STATUS "SDL3 not found, fetching from source...")
            if(NOT DEFINED SDL3_GIT_TAG)
                set(SDL3_GIT_TAG "release-3.2.26")
            endif()
            if(NOT DEFINED SDL3_IMAGE_GIT_TAG)
                set(SDL3_IMAGE_GIT_TAG "release-3.2.4")
            endif()

            FetchContent_Declare(
                SDL3
                GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                GIT_TAG ${SDL3_GIT_TAG}
            )
            FetchContent_Declare(
                SDL3_image
                GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
                GIT_TAG ${SDL3_IMAGE_GIT_TAG}
            )
            FetchContent_MakeAvailable(SDL3 SDL3_image)
        endif()
    endif()

    target_link_libraries(${target_name} PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
    )
endfunction()
