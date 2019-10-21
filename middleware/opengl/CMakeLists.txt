set(MODULE_NAME middleware_opengl)
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
target_link_libraries(${MODULE_NAME} opengl32.lib glog glfw glad)

add_executable(opengl_functest opengl_functest.cc)
target_link_libraries(opengl_functest ${MODULE_NAME} )