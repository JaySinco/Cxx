set(MODULE_NAME product_testbed)
add_custom_target(__target_${MODULE_NAME})

add_executable(testbed_smtp testbed_smtp.cc)
target_link_libraries(testbed_smtp glog gtest libssl libcrypto ws2_32.lib)

add_executable(testbed_window testbed_window.cc)
target_link_libraries(testbed_window common_utility)

add_executable(testbed_imgui testbed_imgui.cc)
target_link_libraries(testbed_imgui glog glfw glad imgui opengl32.lib)

