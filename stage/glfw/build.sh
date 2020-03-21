#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build

cmake -G "Visual Studio 15 2017 Win64" -T "host=x64"  ../glfw-3.3.2 \
    -DCMAKE_INSTALL_PREFIX="${root_dir}/external/windows/x64/glfw" && \
MSBuild.exe -p:Configuration=Release *.sln -maxcpucount $@ && \
MSBuild.exe INSTALL.vcxproj -p:Configuration=Release
