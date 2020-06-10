set(MODULE_NAME common_utility)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS 
    base.h
    base.cpp
    string_helper.h
    string_helper.cpp
    )

add_library(${MODULE_NAME} STATIC ${LIB_SRCS})
target_link_libraries(${MODULE_NAME}
    glog
    gflags
    )
