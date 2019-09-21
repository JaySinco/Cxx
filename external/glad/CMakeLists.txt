set(MODULE_NAME glad)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    src/glad.c
    )
    
add_library(${MODULE_NAME} ${LIB_SRCS})

