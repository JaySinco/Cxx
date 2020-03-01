set(MODULE_NAME product_testbed)
add_custom_target(__target_${MODULE_NAME})

add_executable(testbed testbed.cc)
set_target_properties (testbed PROPERTIES CXX_STANDARD 17)
target_compile_options(testbed PUBLIC "/Zc:__cplusplus")
target_link_libraries(testbed
    glog
    gflags
    shlwapi
    )

add_executable(testbed_screen screen.cc)
target_link_libraries(testbed_screen
    glog
    gflags
    shlwapi
    )

add_executable(testbed_smtp smtp.cc)
target_link_libraries(testbed_smtp
    glog
    gflags
    shlwapi
    gtest
    libssl
    libcrypto
    ws2_32
    )

add_executable(testbed_imgui imgui.cc)
target_link_libraries(testbed_imgui
    glog
    gflags
    shlwapi
    glfw
    glad
    imgui
    opengl32
    )

add_executable(testbed_curl curl.cc)
target_link_libraries(testbed_curl
    ws2_32
    wldap32
    normaliz
    glog
    gflags
    shlwapi
    libcurl
    libssl
    libcrypto
    zlib
    tinyxml
    )

add_executable(testbed_mongo mongo.cc)
target_link_libraries(testbed_mongo
    glog
    gflags
    shlwapi
    mongoc
    bson
    mongocxx
    bsoncxx
    ws2_32
    secur32
    crypt32
    bcrypt
    dnsapi
    )