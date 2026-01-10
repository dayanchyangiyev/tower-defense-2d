include_guard(GLOBAL)

option(PROJECT_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(USE_ASAN "Use Address Sanitizer" OFF)
option(USE_MSAN "Use Memory Sanitizer" OFF)
option(CMAKE_COLOR_DIAGNOSTICS "Enable color diagnostics" ON)

# ------------------------------------------------------------------------------
# Dependency Configuration (Must be set GLOBAL SCOPE before FetchContent)
# ------------------------------------------------------------------------------

# 1. Disable SDL3_image complex formats (Avoids dav1d/NASM dependency on Windows)
# SDL3_image still accepts the old SDLIMAGE_* cache keys, so set both.
set(SDL3IMAGE_AVIF OFF CACHE BOOL "Disable AVIF" FORCE)
set(SDL3IMAGE_AVIF OFF)
set(SDLIMAGE_AVIF OFF CACHE BOOL "Disable AVIF (legacy key)" FORCE)
set(SDLIMAGE_AVIF OFF)

set(SDL3IMAGE_WEBP OFF CACHE BOOL "Disable WEBP" FORCE)
set(SDL3IMAGE_WEBP OFF)
set(SDLIMAGE_WEBP OFF CACHE BOOL "Disable WEBP (legacy key)" FORCE)
set(SDLIMAGE_WEBP OFF)

set(SDL3IMAGE_JXL  OFF CACHE BOOL "Disable JXL" FORCE)
set(SDL3IMAGE_JXL  OFF)
set(SDLIMAGE_JXL  OFF CACHE BOOL "Disable JXL (legacy key)" FORCE)
set(SDLIMAGE_JXL  OFF)

set(SDL3IMAGE_TIF  OFF CACHE BOOL "Disable TIFF" FORCE)
set(SDL3IMAGE_TIF  OFF)
set(SDLIMAGE_TIF  OFF CACHE BOOL "Disable TIFF (legacy key)" FORCE)
set(SDLIMAGE_TIF  OFF)

# 2. Disable SDL3 X11 extensions (Avoids missing headers on minimal Linux CI)
if(UNIX AND NOT APPLE)
    set(SDL_X11_XCURSOR OFF CACHE BOOL "Disable Xcursor" FORCE)
    set(SDL_X11_XCURSOR OFF)
    set(SDL_X11_XINPUT OFF CACHE BOOL "Disable XInput" FORCE)
    set(SDL_X11_XINPUT OFF)
    set(SDL_X11_XRANDR OFF CACHE BOOL "Disable XRandR" FORCE)
    set(SDL_X11_XRANDR OFF)
    set(SDL_X11_XSCRNSAVER OFF CACHE BOOL "Disable XScrnSaver" FORCE)
    set(SDL_X11_XSCRNSAVER OFF)
    set(SDL_X11_XSHAPE OFF CACHE BOOL "Disable XShape" FORCE)
    set(SDL_X11_XSHAPE OFF)
    set(SDL_X11_XFIXES OFF CACHE BOOL "Disable XFixes" FORCE)
    set(SDL_X11_XFIXES OFF)
    set(SDL_X11_XTEST OFF CACHE BOOL "Disable XTest" FORCE)
    set(SDL_X11_XTEST OFF)
    set(SDL_X11_XKB OFF CACHE BOOL "Disable XKB" FORCE)
    set(SDL_X11_XKB OFF)
endif()

# ------------------------------------------------------------------------------

# update name in .github/workflows/cmake.yml:27 when changing "bin" name here
set(DESTINATION_DIR "bin")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install_dir" CACHE PATH "..." FORCE)
endif()

# disable sanitizers when releasing executables without explicitly requested debug info
# use generator expressions to set flags correctly in both single and multi config generators
set(is_debug "$<CONFIG:Debug>")
set(is_rel_with_deb "$<CONFIG:RelWithDebInfo>")
set(debug_mode "$<OR:${is_debug},${is_rel_with_deb}>")
