set(MODULE_NAME product_raytrace)
add_custom_target(__target_${MODULE_NAME})

add_executable(raytrace
    ray.h
    shape.h
    shape.cc
    main.cc
    )
target_link_libraries(raytrace
    common_utility
    opencv_all
    )

