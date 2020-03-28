set(MODULE_NAME product_qt)
add_custom_target(__target_${MODULE_NAME})

add_executable(qt
    main.cc
    widget.h
    widget.cc
    widget.moc.cc
    )

target_link_libraries(qt
    common_utility
    qt-all
    )