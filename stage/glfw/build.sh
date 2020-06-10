#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev \
    libxi-dev libgl1-mesa-dev && \
cmake -G "$generator" ../glfw-3.3.2 \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/glfw" \
&& \
$build && $install
