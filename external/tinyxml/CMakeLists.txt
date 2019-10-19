set(MODULE_NAME tinyxml)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    src/tinyxml2.cpp
    )
    
add_library(${MODULE_NAME} ${LIB_SRCS})

