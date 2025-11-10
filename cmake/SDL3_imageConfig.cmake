include_guard(GLOBAL)

set(_SDL3IMG_ROOT_CANDIDATES "")
if(DEFINED SDL3_IMAGE_ROOT_DIR AND SDL3_IMAGE_ROOT_DIR)
    list(APPEND _SDL3IMG_ROOT_CANDIDATES "${SDL3_IMAGE_ROOT_DIR}")
endif()
if(DEFINED SDL3_ROOT_DIR AND SDL3_ROOT_DIR)
    list(APPEND _SDL3IMG_ROOT_CANDIDATES "${SDL3_ROOT_DIR}")
endif()
list(APPEND _SDL3IMG_ROOT_CANDIDATES
    "${CMAKE_CURRENT_LIST_DIR}/../lib/SDL3"
    "${CMAKE_CURRENT_LIST_DIR}/../../lib/SDL3"
)

set(_SDL3IMG_FRAMEWORK_PATH "")
set(_SDL3IMG_FRAMEWORK_PARENT "")
foreach(_candidate IN LISTS _SDL3IMG_ROOT_CANDIDATES)
    if(_candidate AND EXISTS "${_candidate}/SDL3_image.framework/SDL3_image")
        set(_SDL3IMG_FRAMEWORK_PATH "${_candidate}/SDL3_image.framework")
        set(_SDL3IMG_FRAMEWORK_PARENT "${_candidate}")
        break()
    endif()
endforeach()

if(NOT _SDL3IMG_FRAMEWORK_PATH)
    message(FATAL_ERROR
        "SDL3_imageConfig.cmake could not locate SDL3_image.framework. "
        "Set SDL3_IMAGE_ROOT_DIR to the directory containing SDL3_image.framework")
endif()

if(NOT TARGET SDL3_image::SDL3_image)
    add_library(SDL3_image::SDL3_image SHARED IMPORTED)
    set_target_properties(SDL3_image::SDL3_image PROPERTIES
        IMPORTED_LOCATION "${_SDL3IMG_FRAMEWORK_PATH}/SDL3_image"
        INTERFACE_INCLUDE_DIRECTORIES "${_SDL3IMG_FRAMEWORK_PATH}/Headers"
        FRAMEWORK TRUE
        INTERFACE_LINK_OPTIONS "-F${_SDL3IMG_FRAMEWORK_PARENT}"
    )
endif()

set(SDL3_image_FOUND TRUE)
