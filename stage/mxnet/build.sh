#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build
sudo apt install intel-mkl python
cmake -G "$generator" ../apache-mxnet-src-1.6.0-incubating/ \
    -DUSE_CUDA=0 -DUSE_CUDNN=0 -DUSE_NVRTC=0 -DUSE_OPENCV=1 \
    -DBUILD_CPP_EXAMPLES=0 -DBUILD_TESTING=0 \
    -DUSE_OPENMP=1 -DUSE_PROFILER=1 -DUSE_BLAS=mkl \
    -DUSE_LAPACK=1 -DUSE_DIST_KVSTORE=0 -DCUDA_ARCH_NAME=All \
    -DUSE_MKLDNN=1 -DCMAKE_BUILD_TYPE=Release \
    -DMKL_ROOT="/usr/" \
    -DMKL_INCLUDE_DIR="/usr/include/mkl/" \
    -DOpenCV_DIR="${root_dir}/stage/opencv/build/" \
    -DUSE_CPP_PACKAGE=1 \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/mxnet/" \
&& \
$build && $install
