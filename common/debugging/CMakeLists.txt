set(MODULE_NAME common_debugging)
add_custom_target(__target_${MODULE_NAME})

add_executable(pprint_functest pprint_functest.cpp)
set_target_properties (pprint_functest PROPERTIES CXX_STANDARD 17)
target_link_libraries(pprint_functest)