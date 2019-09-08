set(MODULE_NAME product)
add_custom_target(__target_${MODULE_NAME})

add_subdirectory(testbed)
add_subdirectory(gomoku)

add_dependencies(__target_${MODULE_NAME}
    ${SUBDIRECTORY_TARGETS}
    )