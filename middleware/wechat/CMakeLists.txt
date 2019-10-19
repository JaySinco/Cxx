set(MODULE_NAME middleware_wechat)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    client
    client.cc
    )

add_library(${MODULE_NAME} STATIC ${LIB_SRCS})
target_link_libraries(${MODULE_NAME} Ws2_32.lib wldap32.lib Normaliz.lib 
    glog libcurl libssl libcrypto zlib tinyxml)

add_executable(wechat_functest wechat_functest.cc)
target_link_libraries(wechat_functest ${MODULE_NAME})


