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
    libcurl
    libssl
    libcrypto
    zlib
    ws2_32
    wldap32
    normaliz
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

add_executable(ui_automation_t ui_automation.cc)
target_link_libraries(ui_automation_t
    common_utility
    )

add_executable(screen_locker screen_locker.cc)
target_link_libraries(screen_locker
    common_utility
    )

add_executable(remote_shell
    gen-cpp/remote_shell_types.cpp
    gen-cpp/RemoteShellService.cpp
    remote_shell.cc
    )
target_compile_definitions(remote_shell PUBLIC BOOST_ALL_NO_LIB)
target_link_libraries(remote_shell
    common_utility
    thrift
    boost-atomic
    pdh
    )

add_executable(queue_t queue.cc)
target_link_libraries(queue_t
    common_utility
    gtest
    )

add_executable(qt_t qt.cc)
target_link_libraries(qt_t
    common_utility
    qt-all
    )
