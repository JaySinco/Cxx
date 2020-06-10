set(MODULE_NAME external)
add_custom_target( __target_${MODULE_NAME})

add_library(gflags STATIC IMPORTED GLOBAL)
set_target_properties(gflags PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/gflags/lib/libgflags_nothreads.a)

add_library(glog STATIC IMPORTED GLOBAL)
set_target_properties(glog PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/glog/lib/libglog.a)

add_library(gtest STATIC IMPORTED GLOBAL)
set_target_properties(gtest PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/gtest/lib/libgtest.a)

add_library(libssl STATIC IMPORTED GLOBAL)
set_target_properties(libssl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/openssl/lib/libssl.a)

add_library(libcrypto STATIC IMPORTED GLOBAL)
set_target_properties(libcrypto PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/openssl/lib/libcrypto.a)

add_library(libmxnet STATIC IMPORTED GLOBAL)
set_target_properties(libmxnet PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mxnet/lib/libmxnet.so)

add_library(glfw STATIC IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/glfw/lib/libglfw3.a)

add_library(thrift STATIC IMPORTED GLOBAL)
set_target_properties(thrift PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/thrift/lib/libthrift.a)

add_library(thriftnb STATIC IMPORTED GLOBAL)
set_target_properties(thriftnb PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/thrift/lib/libthriftnb.a)

add_library(zlib STATIC IMPORTED GLOBAL)
set_target_properties(zlib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/zlib/lib/libz.a)

add_library(libcurl STATIC IMPORTED GLOBAL)
set_target_properties(libcurl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/curl/lib/libcurl.a)

add_library(boost-atomic STATIC IMPORTED GLOBAL)
set_target_properties(boost-atomic PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/boost/lib/libboost_atomic.a)

add_library(boost-filesystem STATIC IMPORTED GLOBAL)
set_target_properties(boost-filesystem PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/boost/lib/libboost_filesystem.a)

add_library(mongoc STATIC IMPORTED GLOBAL)
set_target_properties(mongoc PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/libmongoc-static-1.0.a)

add_library(bson STATIC IMPORTED GLOBAL)
set_target_properties(bson PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/libbson-static-1.0.a)

add_library(mongocxx STATIC IMPORTED GLOBAL)
set_target_properties(mongocxx PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/libmongocxx-static.a)

add_library(bsoncxx STATIC IMPORTED GLOBAL)
set_target_properties(bsoncxx PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/libbsoncxx-static.a)

add_library(mongo-all STATIC ${CMAKE_SOURCE_DIR}/.trash.c)
target_link_libraries(mongo-all bson mongoc bsoncxx mongocxx)

add_subdirectory(glad)
add_subdirectory(lua)
add_subdirectory(tinyobjloader)

add_dependencies(__target_${MODULE_NAME}
    ${SUBDIRECTORY_TARGETS}
    )