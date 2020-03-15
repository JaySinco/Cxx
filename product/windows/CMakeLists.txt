set(MODULE_NAME product_windows)
add_custom_target(__target_${MODULE_NAME})

add_executable(screen_locker_functest
    screen_locker.h
    screen_locker.cpp
    screen_locker_functest.cpp
    )
target_link_libraries(screen_locker_functest
    common_utility
    )

add_executable(uiautomation_helper_functest
    uiautomation_helper.h
    uiautomation_helper.cpp
    uiautomation_helper_functest.cpp
    )
target_link_libraries(uiautomation_helper_functest
    common_utility
    )
