#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build

cmake -G "Visual Studio 15 2017 Win64" -T "host=x64" ../opencv-3.4.9 \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_TEST=OFF \
    -DCMAKE_INSTALL_PREFIX="${root_dir}/external/windows/x64/opencv" \
    -DBUILD_PERF_TESTS=OFF && \
MSBuild.exe -p:Configuration=Release *.sln -maxcpucount $@ && \
MSBuild.exe INSTALL.vcxproj -p:Configuration=Release
