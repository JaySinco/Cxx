set(MODULE_NAME product_raytrace)
add_custom_target(__target_${MODULE_NAME})

add_executable(raytrace
    base.h
    base.cc
    ray.h
    camera.h
    camera.cc
    material.h
    material.cc
    shape.h
    shape.cc
    scenery.h
    scenery.cc
    render.h
    render.cc
    main.cc
    )
target_link_libraries(raytrace
    common_utility
    opencv_all
    )

