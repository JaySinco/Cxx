set(MODULE_NAME common)
add_custom_target(__target_${MODULE_NAME})

add_subdirectory(debugging)

add_dependencies(__target_${MODULE_NAME}
    ${SUBDIRECTORY_TARGETS}
    )