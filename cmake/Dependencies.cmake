include_guard(GLOBAL)

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

    endif()

    find_package(SDL3 CONFIG REQUIRED)
    find_package(SDL3_image CONFIG REQUIRED)

    target_link_libraries(${target_name} PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
    )
endfunction()
