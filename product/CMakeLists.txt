set(MODULE_NAME product)
add_custom_target(__target_${MODULE_NAME})

add_subdirectory(gomoku)
add_subdirectory(opengl)
add_subdirectory(remote_shell)
add_subdirectory(screen_locker)
add_subdirectory(testbed)
add_subdirectory(ui_automation)

add_dependencies(__target_${MODULE_NAME}
    ${SUBDIRECTORY_TARGETS}
    )