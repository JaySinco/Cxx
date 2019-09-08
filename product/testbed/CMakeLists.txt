set(MODULE_NAME product_testbed)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    testbed.cc
    )

add_executable(testbed ${LIB_SRCS})
target_link_libraries(testbed glog gtest common_utility)
