#!/bin/bash

_dir=`dirname $(readlink -f $0)`
current_dir=`readlink -f $_dir`
function find_root() {
    _pwd=`readlink -f "${1}"`
    _dep="${2}"
    >&2 echo "$_dep: $_pwd"
    sleep 0.1
    if [ -f "$_pwd/.root" ]
    then
        readlink -f $_pwd
    else
        find_root "$_pwd/../" $(expr $_dep + 1)
    fi
}
root_dir=`find_root "${current_dir}" 0`

MSVC_PATH="/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/HostX86/x86:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/VC/VCPackages:/c/Program Files (x86)/Microsoft SDKs/TypeScript/3.1:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TestWindow:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/bin/Roslyn:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Team Tools/Performance Tools:/c/Program Files (x86)/Microsoft Visual Studio/Shared/Common/VSPerfCollectionTools/:/c/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.6.1 Tools/:/c/Program Files (x86)/Windows Kits/10/bin/10.0.17763.0/x86:/c/Program Files (x86)/Windows Kits/10/bin/x86:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community//MSBuild/15.0/bin:/c/Windows/Microsoft.NET/Framework/v4.0.30319:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/Tools/"
PATH="${MSVC_PATH};${PATH}"

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
