set(MODULE_NAME common_utility)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
	system_info.h
	system_info.cc
    )

add_library(${MODULE_NAME} ${LIB_SRCS})
target_link_libraries(${MODULE_NAME} Pdh.lib)

add_executable(system_info_functest system_info_functest.cc)
target_link_libraries(system_info_functest ${MODULE_NAME})