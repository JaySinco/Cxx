set(MODULE_NAME liblua)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    src/lapi.c
    src/lauxlib.c
    src/lbaselib.c
    src/lbitlib.c
    src/lcode.c
    src/lcorolib.c
    src/lctype.c
    src/ldblib.c
    src/ldebug.c
    src/ldo.c
    src/ldump.c
    src/lfunc.c
    src/lgc.c
    src/linit.c
    src/liolib.c
    src/llex.c
    src/lmathlib.c
    src/lmem.c
    src/loadlib.c
    src/lobject.c
    src/lopcodes.c
    src/loslib.c
    src/lparser.c
    src/lstate.c
    src/lstring.c
    src/lstrlib.c
    src/ltable.c
    src/ltablib.c
    src/ltm.c
    src/lundump.c
    src/lutf8lib.c
    src/lvm.c
    src/lzio.c
    )
add_library(${MODULE_NAME} ${LIB_SRCS})
target_compile_options(${MODULE_NAME} PRIVATE /TP /W2 /EHsc)

# add_executable(lua src/lua.c)
# target_link_libraries(lua ${MODULE_NAME})

# add_executable(luac src/luac.c)
# target_link_libraries(luac ${MODULE_NAME})