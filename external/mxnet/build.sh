#!/bin/bash

_dir=`dirname $(readlink -f $0)`
MSVC_PATH="/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/HostX86/x86:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/VC/VCPackages:/c/Program Files (x86)/Microsoft SDKs/TypeScript/3.1:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TestWindow:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/bin/Roslyn:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Team Tools/Performance Tools:/c/Program Files (x86)/Microsoft Visual Studio/Shared/Common/VSPerfCollectionTools/:/c/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.6.1 Tools/:/c/Program Files (x86)/Windows Kits/10/bin/10.0.17763.0/x86:/c/Program Files (x86)/Windows Kits/10/bin/x86:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community//MSBuild/15.0/bin:/c/Windows/Microsoft.NET/Framework/v4.0.30319:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/Tools/"
PATH="${MSVC_PATH}:${PATH}"
mkdir -p build
cd build
MKL_DLL_PATH="/c/Program Files (x86)/IntelSWTools/compilers_and_libraries_2020.0.166/windows/redist/intel64_win/mkl"
mkdir -p Release
# 编译过程中会报错一次，需要手动将编译好的mkldnn.dll放到Release中
cmake -G "Visual Studio 15 2017 Win64" -T "host=x64" ../apache-mxnet-src-1.6.0-incubating/ \
    -DUSE_CUDA=0 -DUSE_CUDNN=0 -DUSE_NVRTC=0 -DUSE_OPENCV=1 \
    -DBUILD_CPP_EXAMPLES=0 -DBUILD_TESTING=0 \
    -DUSE_OPENMP=1 -DUSE_PROFILER=1 -DUSE_BLAS=mkl \
    -DUSE_LAPACK=1 -DUSE_DIST_KVSTORE=0 -DCUDA_ARCH_NAME=All \
    -DUSE_MKLDNN=1 -DCMAKE_BUILD_TYPE=Release \
    -DMKL_ROOT="C:\Program Files (x86)\IntelSWTools\compilers_and_libraries\windows\mkl" \
    -DOpenCV_DIR=`readlink -f ../../opencv/opencv-3.4.9/build` \
    -DUSE_CPP_PACKAGE=1 -DCMAKE_INSTALL_PREFIX="./mxnet" && \
MSBuild.exe -p:Configuration=Release *.sln -maxcpucount $@ && \
MSBuild.exe INSTALL.vcxproj -p:Configuration=Release
