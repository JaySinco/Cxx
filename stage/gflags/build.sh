#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build
cmake -G "$generator" ../gflags-2.2.2 \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/gflags" \
&& \
$build && $install