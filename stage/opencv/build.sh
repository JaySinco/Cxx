#!/bin/bash

source ../set_env.sh
sudo apt install libgtk2.0-dev libcanberra-gtk-module
mkdir -p build
cd build
cmake -G "$generator" ../opencv-3.4.9 \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_TEST=OFF \
    -DWITH_GTK=ON \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/opencv" \
    -DBUILD_PERF_TESTS=OFF \
&& \
$build && $install
