set(MODULE_NAME common_opengl)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    shader.cc
    )

add_library(${MODULE_NAME} ${LIB_SRCS})
target_link_libraries(${MODULE_NAME} glog glfw glad opengl32.lib)

add_executable(opengl_functest opengl_functest.cc)
target_link_libraries(opengl_functest ${MODULE_NAME} opencv)