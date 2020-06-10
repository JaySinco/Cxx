#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build
cmake -G "$generator" ../zlib-1.2.11 \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/zlib" \
&& \
$build && $install
