set(MODULE_NAME common_utility)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
	sysinfo_utility_win.h
	sysinfo_utility_win.cc
    )

add_library(${MODULE_NAME} ${LIB_SRCS})
target_link_libraries(${MODULE_NAME} Pdh.lib)

add_executable(sysinfo_utility_win_unitest sysinfo_utility_win_unitest.cc)
target_link_libraries(sysinfo_utility_win_unitest ${MODULE_NAME})

add_executable(mail_smtp_unittest mail_smtp_unittest.cc)
target_link_libraries(mail_smtp_unittest glog gtest libssl libcrypto ws2_32.lib)

