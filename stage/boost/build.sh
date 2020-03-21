#!/bin/bash

source ../set_env.sh
cd boost_1_71_0

# ./bootstrap.bat
./b2.exe --with-system --with-thread --with-filesystem --with-chrono\
    --with-program_options --with-test --with-timer --with-atomic \
    --prefix=${root_dir}/external/windows/x64/boost/ \
    toolset=msvc variant=release link=static \
    architecture=x86 address-model=64 threading=multi \
    runtime-link=shared && \
mkdir -p ${root_dir}/external/windows/x64/boost/include/ && \
mkdir -p ${root_dir}/external/windows/x64/boost/lib/ && \
cp -r stage/lib/*.lib ${root_dir}/external/windows/x64/boost/lib/