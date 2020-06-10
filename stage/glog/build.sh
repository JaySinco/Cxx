#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build
cmake -G "$generator" ../glog-0.4.0 \
    -DWITH_GFLAGS=on \
    -Dgflags_DIR="${external_dir}/gflags" \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/glog" \
&& \
$build && $install
