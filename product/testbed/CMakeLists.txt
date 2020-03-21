set(MODULE_NAME product_testbed)
add_custom_target(__target_${MODULE_NAME})

add_executable(testbed testbed.cc)
set_target_properties (testbed PROPERTIES CXX_STANDARD 17)
target_compile_options(testbed PUBLIC "/Zc:__cplusplus")
target_link_libraries(testbed
    common_utility
    )

add_executable(socket_t socket.cc)
target_link_libraries(socket_t
    common_utility
    gtest
    libssl
    libcrypto
    ws2_32
    )

add_executable(imgui_t imgui.cc)
target_link_libraries(imgui_t
    common_utility
    glfw
    glad
    imgui
    opengl32
    )

add_executable(http_t http.cc)
target_link_libraries(http_t
    common_utility
    ws2_32
    wldap32
    normaliz
    libcurl
    libssl
    libcrypto
    zlib
    tinyxml
    )

add_executable(mongo_t mongo.cc)
target_link_libraries(mongo_t
    common_utility
    mongo-all
    ws2_32
    secur32
    crypt32
    bcrypt
    dnsapi
    )

add_executable(lua_t lua.cc)
target_link_libraries(lua_t
    common_utility
    liblua
    )