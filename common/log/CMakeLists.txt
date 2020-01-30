set(MODULE_NAME common_log)
add_custom_target(__target_${MODULE_NAME})

add_executable(pprint_functest pprint_functest.cpp)
target_link_libraries(pprint_functest)
set_property (TARGET pprint_functest
    PROPERTY
    # Enable C++17 standard compliance
    CXX_STANDARD 17
)