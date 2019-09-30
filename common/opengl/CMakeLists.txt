set(MODULE_NAME common_opengl)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    shader.h
    shader.cc
    texture.h
    texture.cc
    object.h
    object.cc
    )

add_library(${MODULE_NAME} ${LIB_SRCS})
target_link_libraries(${MODULE_NAME} opengl32.lib glog glfw glad opencv)

add_executable(opengl_functest opengl_functest.cc)
target_link_libraries(opengl_functest ${MODULE_NAME} )