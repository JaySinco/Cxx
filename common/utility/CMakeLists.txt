set(MODULE_NAME common_utility)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
	sysinfo_win.h
	sysinfo_win.cc
    )

add_library(${MODULE_NAME} ${LIB_SRCS})
target_link_libraries(${MODULE_NAME} Pdh.lib)

add_executable(sysinfo_win_functest sysinfo_win_functest.cc)
target_link_libraries(sysinfo_win_functest ${MODULE_NAME})