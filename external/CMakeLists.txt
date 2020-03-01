set(MODULE_NAME external)
add_custom_target( __target_${MODULE_NAME})

add_library(gflags STATIC IMPORTED GLOBAL)
set_target_properties(gflags PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/gflags/lib/gflags_static.lib)

add_library(glog STATIC IMPORTED GLOBAL)
set_target_properties(glog PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/glog/lib/glog.lib)

add_library(gtest STATIC IMPORTED GLOBAL)
set_target_properties(gtest PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/gtest/lib/gtest.lib)

add_library(libssl STATIC IMPORTED GLOBAL)
set_target_properties(libssl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/openssl/lib/libssl_static.lib)

add_library(libcrypto STATIC IMPORTED GLOBAL)
set_target_properties(libcrypto PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/openssl/lib/libcrypto_static.lib)

add_library(libmxnet STATIC IMPORTED GLOBAL)
set_target_properties(libmxnet PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mxnet/lib/libmxnet.lib)

add_library(glfw STATIC IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/glfw/lib/glfw3.lib)

add_library(opencv STATIC IMPORTED GLOBAL)
set_target_properties(opencv PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/opencv/lib/opencv_world347.lib)

add_library(assimp STATIC IMPORTED GLOBAL)
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/assimp/lib/assimp-vc141-mt.lib)

add_library(thrift STATIC IMPORTED GLOBAL)
set_target_properties(thrift PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/thrift/lib/thriftmd.lib)

add_library(thriftnb STATIC IMPORTED GLOBAL)
set_target_properties(thriftnb PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/thrift/lib/thriftnbmd.lib)

add_library(libcurl STATIC IMPORTED GLOBAL)
set_target_properties(libcurl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/curl/lib/libcurl_a.lib)

add_library(zlib STATIC IMPORTED GLOBAL)
set_target_properties(zlib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/zlib/lib/zlibstatic.lib)

add_library(leveldb STATIC IMPORTED GLOBAL)
set_target_properties(leveldb PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/leveldb/lib/leveldb.lib)

add_library(boost-atomic STATIC IMPORTED GLOBAL)
set_target_properties(boost-atomic PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/boost/lib/libboost_atomic-vc141-mt-x64-1_71.lib)

add_library(mongoc STATIC IMPORTED GLOBAL)
set_target_properties(mongoc PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/mongoc-static-1.0.lib)

add_library(bson STATIC IMPORTED GLOBAL)
set_target_properties(bson PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/bson-static-1.0.lib)

add_library(mongocxx STATIC IMPORTED GLOBAL)
set_target_properties(mongocxx PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/mongocxx-static.lib)

add_library(bsoncxx STATIC IMPORTED GLOBAL)
set_target_properties(bsoncxx PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mongo/lib/bsoncxx-static.lib)

add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(tinyxml)

add_dependencies(__target_${MODULE_NAME}
    ${SUBDIRECTORY_TARGETS}
    )