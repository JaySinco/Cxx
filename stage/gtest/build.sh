#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build
cmake -G "$generator" ../googletest-1.8.1 \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/gtest" \
&& \
$build && $install
