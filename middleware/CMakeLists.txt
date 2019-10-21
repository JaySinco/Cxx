set(MODULE_NAME middleware)
add_custom_target(__target_${MODULE_NAME})

add_subdirectory(opengl)

add_dependencies(__target_${MODULE_NAME}
    ${SUBDIRECTORY_TARGETS}
    )