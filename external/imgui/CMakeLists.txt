set(MODULE_NAME imgui)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    src/imgui.cpp
    src/imgui_demo.cpp
    src/imgui_draw.cpp
    src/imgui_impl_glfw.cpp
    src/imgui_impl_opengl3.cpp
    src/imgui_widgets.cpp
    )
    
add_library(${MODULE_NAME} ${LIB_SRCS})
target_compile_definitions(${MODULE_NAME} PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLAD
    )
