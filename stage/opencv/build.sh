#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build
cmake -G "$generator" ../opencv-3.4.9 \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_TEST=OFF \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/opencv" \
    -DBUILD_PERF_TESTS=OFF \
&& \
$build && $install
