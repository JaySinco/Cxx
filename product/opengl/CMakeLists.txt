set(MODULE_NAME product_opengl)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    base.h
    camera.h
    camera.cc
    light.h
    material.h
    model.h
    model.cc
    object.h
    object.cc
    scenery.h
    scenery.cc
    shader.h
    shader.cc
    storage.h
    texture.h
    texture.cc
    )

add_library(${MODULE_NAME} ${LIB_SRCS})
target_link_libraries(${MODULE_NAME}
    common_utility
    tinyobjloader
    GL
    glfw
    glad
    X11
    dl
    pthread
    )

add_executable(objviewer objviewer.cc)
target_link_libraries(objviewer ${MODULE_NAME})