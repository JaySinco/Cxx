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

add_executable(socket_t socket.cc)
target_link_libraries(socket_t
    glog
    gflags
    shlwapi
    gtest
    libssl
    libcrypto
    ws2_32
    )

add_executable(imgui_t imgui.cc)
target_link_libraries(imgui_t
    glog
    gflags
    shlwapi
    glfw
    glad
    imgui
    opengl32
    )

add_executable(http_t http.cc)
target_link_libraries(http_t
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

add_executable(mongo_t mongo.cc)
target_link_libraries(mongo_t
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