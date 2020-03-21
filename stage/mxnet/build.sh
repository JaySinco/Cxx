#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build

# 编译过程中会报错一次，需要手动将编译好的mkldnn.dll和mkl_rt.dll放到Release中
MKL_DLL_PATH="/c/Program Files (x86)/IntelSWTools/compilers_and_libraries_2020.0.166/windows/redist/intel64_win/mkl"
cmake -G "Visual Studio 15 2017 Win64" -T "host=x64" ../apache-mxnet-src-1.6.0-incubating/ \
    -DUSE_CUDA=0 -DUSE_CUDNN=0 -DUSE_NVRTC=0 -DUSE_OPENCV=1 \
    -DBUILD_CPP_EXAMPLES=0 -DBUILD_TESTING=0 \
    -DUSE_OPENMP=1 -DUSE_PROFILER=1 -DUSE_BLAS=mkl \
    -DUSE_LAPACK=1 -DUSE_DIST_KVSTORE=0 -DCUDA_ARCH_NAME=All \
    -DUSE_MKLDNN=1 -DCMAKE_BUILD_TYPE=Release \
    -DMKL_ROOT="C:\Program Files (x86)\IntelSWTools\compilers_and_libraries\windows\mkl" \
    -DOpenCV_DIR="${root_dir}/stage/opencv/build/" \
    -DUSE_CPP_PACKAGE=1 \
    -DCMAKE_INSTALL_PREFIX="${root_dir}/external/windows/x64/mxnet/" && \
MSBuild.exe -p:Configuration=Release *.sln -maxcpucount $@ && \
MSBuild.exe INSTALL.vcxproj -p:Configuration=Release
