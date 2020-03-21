#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build

cmake -G "Visual Studio 15 2017 Win64" -T "host=x64"  ../curl-7.66.0 \
    -DOPENSSL_ROOT_DIR="${root_dir}/external/windows/x64/openssl/" \
    -DOPENSSL_CRYPTO_LIBRARY="${root_dir}/external/windows/x64/openssl/lib/libcrypto.lib" \
    -DOPENSSL_INCLUDE_DIR="${root_dir}/external/windows/x64/openssl/include/" \
    -DZLIB_ROOT="${root_dir}/external/windows/x64/zlib/" \
    -DZLIB_INCLUDE_DIR="${root_dir}/external/windows/x64/zlib/include/" \
    -DZLIB_LIBRARY="${root_dir}/external/windows/x64/zlib/lib/zlibstatic.lib" \
    -DCMAKE_INSTALL_PREFIX="${root_dir}/external/windows/x64/curl" \
    -DBUILD_SHARED_LIBS=off \
    -DCMAKE_USE_OPENSSL=on && \
MSBuild.exe -p:Configuration=Release *.sln -maxcpucount $@ && \
MSBuild.exe INSTALL.vcxproj -p:Configuration=Release
