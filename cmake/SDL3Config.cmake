include_guard(GLOBAL)

set(_SDL3_ROOT_CANDIDATES "")
if(DEFINED SDL3_ROOT_DIR AND SDL3_ROOT_DIR)
    list(APPEND _SDL3_ROOT_CANDIDATES "${SDL3_ROOT_DIR}")
endif()
list(APPEND _SDL3_ROOT_CANDIDATES
    "${CMAKE_CURRENT_LIST_DIR}/../lib/SDL3"
    "${CMAKE_CURRENT_LIST_DIR}/../../lib/SDL3"
)

set(_SDL3_FRAMEWORK_PATH "")
set(_SDL3_FRAMEWORK_PARENT "")
foreach(_candidate IN LISTS _SDL3_ROOT_CANDIDATES)
    if(_candidate AND EXISTS "${_candidate}/SDL3.framework/SDL3")
        set(_SDL3_FRAMEWORK_PATH "${_candidate}/SDL3.framework")
        set(_SDL3_FRAMEWORK_PARENT "${_candidate}")
        break()
    endif()
endforeach()

if(NOT _SDL3_FRAMEWORK_PATH)
    message(FATAL_ERROR
        "SDL3Config.cmake could not locate SDL3.framework. "
        "Set SDL3_ROOT_DIR to the directory containing SDL3.framework")
endif()

if(NOT TARGET SDL3::SDL3)
    add_library(SDL3::SDL3 SHARED IMPORTED)
    set_target_properties(SDL3::SDL3 PROPERTIES
        IMPORTED_LOCATION "${_SDL3_FRAMEWORK_PATH}/SDL3"
        INTERFACE_INCLUDE_DIRECTORIES "${_SDL3_FRAMEWORK_PATH}/Headers"
        FRAMEWORK TRUE
        INTERFACE_LINK_OPTIONS "-F${_SDL3_FRAMEWORK_PARENT}"
    )
endif()

set(SDL3_FOUND TRUE)
