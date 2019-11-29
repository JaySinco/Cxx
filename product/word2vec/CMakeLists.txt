set(MODULE_NAME product_word2vec)
add_custom_target(__target_${MODULE_NAME})

add_executable(word2vec main.cc)
target_link_libraries(word2vec leveldb)
