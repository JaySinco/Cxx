set(MODULE_NAME product_remote_shell)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS 
    gen-cpp/service_types.cpp
    gen-cpp/RemoteShellService.cpp
    service.cc
    )

add_library(${MODULE_NAME} STATIC ${LIB_SRCS})
target_compile_definitions(${MODULE_NAME} PUBLIC BOOST_ALL_NO_LIB)
target_link_libraries(${MODULE_NAME}
    common_utility
    thrift
    boost-atomic
    Advapi32
    pdh
    )

add_executable(remote_shell remote_shell.cc)
target_link_libraries(remote_shell ${MODULE_NAME})
