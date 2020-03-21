set(MODULE_NAME product_ui_automation)
add_custom_target(__target_${MODULE_NAME})

add_executable(uiautomation_helper_functest
    uiautomation_helper.h
    uiautomation_helper.cpp
    uiautomation_helper_functest.cpp
    )
target_link_libraries(uiautomation_helper_functest
    common_utility
    )
