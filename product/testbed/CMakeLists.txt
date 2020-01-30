set(MODULE_NAME product_testbed)
add_custom_target(__target_${MODULE_NAME})

add_executable(testbed testbed.cc)
target_link_libraries(testbed)

add_executable(testbed_smtp testbed_smtp.cc)
target_link_libraries(testbed_smtp glog gtest libssl libcrypto ws2_32.lib)

add_executable(testbed_imgui testbed_imgui.cc)
target_link_libraries(testbed_imgui glog glfw glad imgui opengl32.lib)

add_executable(testbed_curl testbed_curl.cc)
target_link_libraries(testbed_curl Ws2_32.lib wldap32.lib Normaliz.lib 
    glog libcurl libssl libcrypto zlib tinyxml)