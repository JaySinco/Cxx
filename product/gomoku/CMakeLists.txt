set(MODULE_NAME product_gomoku)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    src/mcts.h 
    src/game.h 
    src/network.h 
    src/vars.h 
    src/train.h
    src/main.cc 
    src/mcts.cc 
    src/game.cc 
    src/network.cc 
    src/train.cc
    )

add_executable(gomoku ${LIB_SRCS})
target_link_libraries(gomoku libmxnet)

set(COPY_FILE_LIST
    ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mxnet/bin/libmxnet.dll
    ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mxnet/bin/mkldnn.dll
    ${CMAKE_SOURCE_DIR}/product/gomoku/model/FIR-8x64i3@9336.param
    )

add_custom_command(TARGET gomoku
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${COPY_FILE_LIST}
    $<TARGET_FILE_DIR:gomoku>
    )