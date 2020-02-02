set(MODULE_NAME product_testbed)
add_custom_target(__target_${MODULE_NAME})

add_executable(testbed hello_world.cc)
target_link_libraries(testbed)

add_executable(testbed_smtp smtp.cc)
target_link_libraries(testbed_smtp glog gtest libssl libcrypto ws2_32.lib)

add_executable(testbed_imgui imgui.cc)
target_link_libraries(testbed_imgui glog glfw glad imgui opengl32.lib)

add_executable(testbed_curl curl.cc)
target_link_libraries(testbed_curl Ws2_32.lib wldap32.lib Normaliz.lib 
    glog libcurl libssl libcrypto zlib tinyxml)