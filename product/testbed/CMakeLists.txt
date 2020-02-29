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
    Ws2_32
    wldap32
    Normaliz 
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
    mongocxx
    bsoncxx
    )

set(WINDOWS_DLL_LIST
    ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/bin/bsoncxx.dll
    ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/bin/mongocxx.dll
    ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/bin/libbson-1.0.dll
    ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/bin/libmongoc-1.0.dll
    )

add_custom_command(TARGET testbed_mongo
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${WINDOWS_DLL_LIST}
    $<TARGET_FILE_DIR:testbed_mongo>
    )