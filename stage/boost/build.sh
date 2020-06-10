#!/bin/bash

source ../set_env.sh
cd boost_1_73_0
if [ ! -f "b2" ]; then
    ./bootstrap.sh
fi
./b2 --with-system --with-thread --with-filesystem --with-chrono \
    --with-program_options --with-test --with-timer --with-atomic \
    --prefix=${external_dir}/boost/ \
    toolset=gcc variant=release link=static \
    architecture=x86 address-model=64 threading=multi install