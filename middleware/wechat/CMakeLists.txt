cmake_minimum_required(VERSION 3.2)
project(CXX_Wechat)

add_compile_options(/source-charset:utf-8)
# include_directories(../../windows/x64/include)
# link_directories(D:/Jaysinco/Cxx/lib)

add_executable(wechat main.cc)

# set_property(TARGET wechat PROPERTY CXX_STANDARD 11)
# target_link_libraries(wechat libmxnet.lib)


