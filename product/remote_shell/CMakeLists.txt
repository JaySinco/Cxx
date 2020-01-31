set(MODULE_NAME product_remote_shell)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS 
    gen-cpp/remote_shell_service_types.cpp
    gen-cpp/RemoteShellService.cpp
    
    remote_shell_service.cc
    )

add_library(${MODULE_NAME} STATIC ${LIB_SRCS})
target_link_libraries(${MODULE_NAME} Advapi32.lib thrift boost-atomic glog)

add_executable(remote_shell remote_shell.cc)
target_compile_definitions(remote_shell PRIVATE BOOST_ALL_NO_LIB)
target_link_libraries(remote_shell ${MODULE_NAME})

add_dependencies(__target_${MODULE_NAME} 
    ${SUBDIRECTORY_TARGETS}
    )
