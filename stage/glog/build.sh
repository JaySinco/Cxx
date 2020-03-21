#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build

cmake -G "Visual Studio 15 2017 Win64" -T "host=x64"  ../glog-0.4.0 \
    -DBUILD_SHARED_LIBS=off \
    -DWITH_GFLAGS=on \
    -Dgflags_DIR="${root_dir}/external/windows/x64/gflags" \
    -DCMAKE_INSTALL_PREFIX="${root_dir}/external/windows/x64/glog" && \
MSBuild.exe -p:Configuration=Release *.sln -maxcpucount $@ && \
MSBuild.exe INSTALL.vcxproj -p:Configuration=Release
