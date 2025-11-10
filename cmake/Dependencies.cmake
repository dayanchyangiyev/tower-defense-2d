include_guard(GLOBAL)

function(setup_sdl_dependencies target_name)
    if(APPLE)
        set(SDL_FRAMEWORK_ROOT "${CMAKE_SOURCE_DIR}/lib/SDL3" CACHE PATH "Location of bundled SDL3 frameworks")

        find_library(SDL3_FRAMEWORK SDL3
            REQUIRED
            PATHS "${SDL_FRAMEWORK_ROOT}"
            NO_DEFAULT_PATH
        )
        find_library(SDL3_IMAGE_FRAMEWORK SDL3_image
            REQUIRED
            PATHS "${SDL_FRAMEWORK_ROOT}"
            NO_DEFAULT_PATH
        )

        target_include_directories(${target_name} PRIVATE
            "${SDL3_FRAMEWORK}/Headers"
            "${SDL3_IMAGE_FRAMEWORK}/Headers"
        )

        target_link_libraries(${target_name} PRIVATE
            "${SDL3_FRAMEWORK}"
            "${SDL3_IMAGE_FRAMEWORK}"
        )
    else()
        find_package(SDL3 CONFIG REQUIRED)
        find_package(SDL3_image CONFIG REQUIRED)

        target_link_libraries(${target_name} PRIVATE
            SDL3::SDL3
            SDL3_image::SDL3_image
        )
    endif()
endfunction()
