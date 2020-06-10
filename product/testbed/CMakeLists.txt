set(MODULE_NAME product_testbed)
add_custom_target(__target_${MODULE_NAME})

add_executable(lua_t lua.cc)
target_link_libraries(lua_t
    common_utility
    lua-core
    )

