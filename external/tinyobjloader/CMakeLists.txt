set(MODULE_NAME tinyobjloader)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    src/tiny_obj_loader.cc
    )
add_library(${MODULE_NAME} ${LIB_SRCS})

