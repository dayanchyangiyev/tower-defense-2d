include_guard(GLOBAL)

include(FetchContent)

function(setup_sdl_dependencies target_name)
    # 1. Try finding installed SDL3 packages FIRST
    find_package(SDL3 CONFIG QUIET)
    find_package(SDL3_image CONFIG QUIET)

    # 2. If not found, download and build via FetchContent
    if(NOT SDL3_FOUND)
        message(STATUS "SDL3 not found. Fetching from GitHub...")
        
        # Disable optional X11 dependencies that are missing in minimal CI environments
        if(UNIX AND NOT APPLE)
            set(SDL_X11_XCURSOR OFF CACHE BOOL "Disable Xcursor" FORCE)
            set(SDL_X11_XINPUT OFF CACHE BOOL "Disable XInput" FORCE)
            set(SDL_X11_XRANDR OFF CACHE BOOL "Disable XRandR" FORCE)
            set(SDL_X11_XSCRNSAVER OFF CACHE BOOL "Disable XScrnSaver" FORCE)
            set(SDL_X11_XSHAPE OFF CACHE BOOL "Disable XShape" FORCE)
            set(SDL_X11_XFIXES OFF CACHE BOOL "Disable XFixes" FORCE)
            set(SDL_X11_XTEST OFF CACHE BOOL "Disable XTest" FORCE)
            set(SDL_X11_XKB OFF CACHE BOOL "Disable XKB" FORCE)
        endif()

        FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG main  # SDL3 is in development
            OVERRIDE_FIND_PACKAGE
        )
        FetchContent_MakeAvailable(SDL3)
    endif()

    if(NOT SDL3_image_FOUND)
        message(STATUS "SDL3_image not found. Fetching from GitHub...")
        FetchContent_Declare(
            SDL3_image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG main
            OVERRIDE_FIND_PACKAGE
        )
        FetchContent_MakeAvailable(SDL3_image)
    endif()

    # 3. Link dependencies
    # SDL3 exports SDL3::SDL3 or SDL3-static depending on build args.
    # FetchContent usually makes SDL3::SDL3 available or just SDL3 target.
    
    if(TARGET SDL3::SDL3)
        target_link_libraries(${target_name} PRIVATE SDL3::SDL3)
    elseif(TARGET SDL3)
        target_link_libraries(${target_name} PRIVATE SDL3)
    else()
        message(FATAL_ERROR "SDL3 target not found even after FetchContent!")
    endif()

    if(TARGET SDL3_image::SDL3_image)
        target_link_libraries(${target_name} PRIVATE SDL3_image::SDL3_image)
    elseif(TARGET SDL3_image)
        target_link_libraries(${target_name} PRIVATE SDL3_image)
    else()
        message(FATAL_ERROR "SDL3_image target not found even after FetchContent!")
    endif()
endfunction()
