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

add_executable(ui_assistant_functest
    ui_assistant.h
    ui_assistant.cpp
    ui_assistant_functest.cpp
    )
target_link_libraries(ui_assistant_functest
    common_utility
    )
