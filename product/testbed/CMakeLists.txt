set(MODULE_NAME product_testbed)
add_custom_target(__target_${MODULE_NAME})

add_executable(testbed testbed.cc)
set_target_properties (testbed PROPERTIES CXX_STANDARD 17)
target_link_libraries(testbed
    glog
    )

add_executable(testbed_smtp smtp.cc)
target_link_libraries(testbed_smtp
    glog
    gtest
    libssl
    libcrypto
    ws2_32
    )

add_executable(testbed_imgui imgui.cc)
target_link_libraries(testbed_imgui
    glog
    glfw
    glad
    imgui
    opengl32
    )

add_executable(testbed_curl curl.cc)
target_link_libraries(testbed_curl
    Ws2_32
    wldap32
    Normaliz 
    glog
    libcurl
    libssl
    libcrypto
    zlib
    tinyxml
    )