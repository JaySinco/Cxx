set(MODULE_NAME external)
add_custom_target( __target_${MODULE_NAME})

add_library(gflags STATIC IMPORTED GLOBAL)
set_target_properties(gflags PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/gflags/lib/gflags_static.lib)

add_library(glog STATIC IMPORTED GLOBAL)
set_target_properties(glog PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/glog/lib/glog.lib)

add_library(gtest STATIC IMPORTED GLOBAL)
set_target_properties(gtest PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/gtest/lib/gtest.lib)

add_library(libssl STATIC IMPORTED GLOBAL)
set_target_properties(libssl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/openssl/lib/libssl.lib)

add_library(libcrypto STATIC IMPORTED GLOBAL)
set_target_properties(libcrypto PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/openssl/lib/libcrypto.lib)

add_library(libmxnet STATIC IMPORTED GLOBAL)
set_target_properties(libmxnet PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mxnet/lib/libmxnet.lib)

add_library(glfw STATIC IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/glfw/lib/glfw3.lib)

add_library(thrift STATIC IMPORTED GLOBAL)
set_target_properties(thrift PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/thrift/lib/thriftmd.lib)

add_library(thriftnb STATIC IMPORTED GLOBAL)
set_target_properties(thriftnb PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/thrift/lib/thriftnbmd.lib)

add_library(zlib STATIC IMPORTED GLOBAL)
set_target_properties(zlib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/zlib/lib/zlibstatic.lib)

add_library(libcurl STATIC IMPORTED GLOBAL)
set_target_properties(libcurl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/curl/lib/libcurl.lib)

add_library(boost-atomic STATIC IMPORTED GLOBAL)
set_target_properties(boost-atomic PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/boost/lib/libboost_atomic-vc141-mt-x64-1_71.lib)

add_library(boost-filesystem STATIC IMPORTED GLOBAL)
set_target_properties(boost-filesystem PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/boost/lib/libboost_filesystem-vc141-mt-x64-1_71.lib)

add_library(mongoc STATIC IMPORTED GLOBAL)
set_target_properties(mongoc PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/mongoc-static-1.0.lib)

add_library(bson STATIC IMPORTED GLOBAL)
set_target_properties(bson PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/bson-static-1.0.lib)

add_library(mongocxx STATIC IMPORTED GLOBAL)
set_target_properties(mongocxx PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/mongocxx-static.lib)

add_library(bsoncxx STATIC IMPORTED GLOBAL)
set_target_properties(bsoncxx PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/bsoncxx-static.lib)

add_library(mongo-all STATIC ${CMAKE_SOURCE_DIR}/.trash.c)
target_link_libraries(mongo-all bson mongoc bsoncxx mongocxx)

add_library(qt-all STATIC ${CMAKE_SOURCE_DIR}/.trash.c)
target_link_libraries(qt-all
    Qt5Core.lib
    Qt5Widgets.lib
    Qt5Gui.lib
    Qt5WindowsUIAutomationSupport.lib
    qtlibpng.lib
    qtharfbuzz.lib
    qwindows.lib
    qtpcre2.lib
    libEGL.lib
    libGLESv2.lib
    Qt5FontDatabaseSupport.lib
    qtfreetype.lib
    translator.lib
    preprocessor.lib
    Qt5EventDispatcherSupport.lib
    Qt5ThemeSupport.lib
    ws2_32.lib
    Netapi32.lib
    version.lib
    winmm.lib
    Userenv.lib
    opengl32.lib
    Dwmapi.lib
    Wtsapi32.lib
    Imm32.lib
    d3d9.lib
    )

add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(tinyxml)
add_subdirectory(lua)
add_subdirectory(tinyobjloader)

add_dependencies(__target_${MODULE_NAME}
    ${SUBDIRECTORY_TARGETS}
    )