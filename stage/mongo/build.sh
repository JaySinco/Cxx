#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build

# 编译mongoc
mkdir -p mongoc
cd mongoc
cmake -G "$generator" ../../mongo-c-driver-1.13.0 \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/mongo/" \
    -DBUILD_VERSION="1.13.0" \
&& \
$build && $install
cd ..

# 编译mongocxx
mkdir -p mongcxx
cd mongcxx
cmake -G "$generator" ../../mongo-cxx-driver-r3.3.1 \
    -DCMAKE_CXX_STANDARD=11 \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/mongo/" \
    -DCMAKE_PREFIX_PATH=`readlink -f ../mongoc/` \
    -DBOOST_ROOT="${external_dir}/boost/" \
    -DBUILD_VERSION="r3.4.0" \
&& \
$build && $install
