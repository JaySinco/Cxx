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

