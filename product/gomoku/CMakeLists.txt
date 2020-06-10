set(MODULE_NAME product_gomoku)
add_custom_target(__target_${MODULE_NAME})

set(LIB_SRCS
    mcts.h 
    game.h 
    network.h 
    vars.h 
    train.h
    main.cc 
    mcts.cc 
    game.cc 
    network.cc 
    train.cc
    )

add_executable(gomoku ${LIB_SRCS})
target_link_libraries(gomoku
    common_utility
    libmxnet
    )

set(COPY_FILE_LIST
    ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mxnet/lib/libmxnet.so
    ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mxnet/lib/libmkldnn.so.1
    ${CMAKE_SOURCE_DIR}/external/${TARGET_OS}/${TARGET_ABI}/mxnet/lib/libomp.so
    )

add_custom_command(TARGET gomoku
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${COPY_FILE_LIST}
    $<TARGET_FILE_DIR:gomoku>
    )