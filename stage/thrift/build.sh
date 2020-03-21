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

cmake -G "Visual Studio 15 2017 Win64" -T "host=x64"  ../thrift-0.12.0 \
    -DLIBEVENT_ROOT="${root_dir}/external/windows/x64/libevent/" \
    -DLIBEVENT_INCLUDE_DIRS="${root_dir}/external/windows/x64/libevent/include/" \
    -DLIBEVENT_LIBRARIES="${root_dir}/external/windows/x64/libevent/lib/"  \
    -DOPENSSL_ROOT_DIR="${root_dir}/external/windows/x64/openssl/" \
    -DOPENSSL_CRYPTO_LIBRARY="${root_dir}/external/windows/x64/openssl/lib/libcrypto.lib" \
    -DOPENSSL_INCLUDE_DIR="${root_dir}/external/windows/x64/openssl/include/" \
    -DBOOST_ROOT="${root_dir}/external/windows/x64/boost/" \
    -DBOOST_LIBRARYDIR="${root_dir}/external/windows/x64/boost/lib/" \
    -DBoost_USE_STATIC_RUNTIME=off \
    -DBoost_COMPILER="-vc141" \
    -DZLIB_ROOT="${root_dir}/external/windows/x64/zlib/" \
    -DZLIB_INCLUDE_DIR="${root_dir}/external/windows/x64/zlib/include/" \
    -DZLIB_LIBRARY="${root_dir}/external/windows/x64/zlib/lib/zlibstatic.lib" \
    -DCMAKE_INSTALL_PREFIX="${root_dir}/external/windows/x64/thrift" \
    -DWITH_SHARED_LIB=off \
    -DWITH_STATIC_LIB=on \
    -DWITH_STDTHREADS=on \
    -DWITH_BOOSTTHREADS=off \
    -DWITH_BOOST_STATIC=on \
    -DWITH_ZLIB=on \
    -DWITH_LIBEVENT=on \
    -DBUILD_TESTING=off \
    -DMSVC_RUNTIME=dynamic && \
MSBuild.exe -p:Configuration=Release *.sln -maxcpucount $@ && \
MSBuild.exe INSTALL.vcxproj -p:Configuration=Release
