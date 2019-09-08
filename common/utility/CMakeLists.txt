set(MODULE_NAME common_utility)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
	sysinfo_utility_win.h
	sysinfo_utility_win.cc
    )

add_library(${MODULE_NAME} ${LIB_SRCS})
target_link_libraries(${MODULE_NAME} Pdh.lib)

add_executable(sysinfo_utility_win_functest sysinfo_utility_win_functest.cc)
target_link_libraries(sysinfo_utility_win_functest ${MODULE_NAME})

add_executable(mail_smtp_functest mail_smtp_functest.cc)
target_link_libraries(mail_smtp_functest glog gtest libssl libcrypto ws2_32.lib)

