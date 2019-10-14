set(MODULE_NAME common_debug)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
	StackWalker.h
	StackWalker.cc
    )

add_library(${MODULE_NAME} ${LIB_SRCS})
target_link_libraries(${MODULE_NAME})

#add_executable(stack_walker_functest stack_walker_functest.cc)
#target_link_libraries(stack_walker_functest ${MODULE_NAME})

add_executable(pprint_functest pprint_functest.cc)
target_link_libraries(pprint_functest ${MODULE_NAME})

set_property (TARGET pprint_functest
  PROPERTY
    # Enable C++17 standard compliance
    CXX_STANDARD 17
)