#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build

# 编译mongoc
mkdir -p mongoc
cd mongoc
cmake -G "Visual Studio 15 2017 Win64" -T "host=x64" ../../mongo-c-driver-1.13.0 \
    -DCMAKE_INSTALL_PREFIX="${root_dir}/external/windows/x64/mongo/" \
    -DBUILD_VERSION="1.13.0" && \
MSBuild.exe -p:Configuration=Release *.sln -maxcpucount $@ && \
MSBuild.exe INSTALL.vcxproj -p:Configuration=Release
cd ..

# 编译mongocxx
mkdir -p mongcxx
cd mongcxx
cmake -G "Visual Studio 15 2017 Win64" -T "host=x64" ../../mongo-cxx-driver-r3.3.1 \
    -DCMAKE_CXX_STANDARD=11 \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_CXX_FLAGS="/Zc:__cplusplus" \
    -DCMAKE_INSTALL_PREFIX="${root_dir}/external/windows/x64/mongo/" \
    -DCMAKE_PREFIX_PATH=`readlink -f ../mongoc/` \
    -DBOOST_ROOT="${root_dir}/external/windows/x64/boost/" \
    -DBUILD_VERSION="r3.4.0" && \
MSBuild.exe -p:Configuration=Release *.sln -maxcpucount $@
MSBuild.exe INSTALL.vcxproj -p:Configuration=Release
