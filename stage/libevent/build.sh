#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build

cmake -G "Visual Studio 15 2017 Win64" -T "host=x64" ../libevent-release-2.1.11-stable \
    -DOPENSSL_ROOT_DIR="${root_dir}/external/windows/x64/openssl/" \
    -DOPENSSL_CRYPTO_LIBRARY="${root_dir}/external/windows/x64/openssl/lib/libcrypto.lib" \
    -DOPENSSL_INCLUDE_DIR="${root_dir}/external/windows/x64/openssl/include/" \
    -DCMAKE_INSTALL_PREFIX="${root_dir}/external/windows/x64/libevent" \
    -DEVENT__LIBRARY_TYPE=static \
    -DMSVC_RUNTIME=dynamic && \
MSBuild.exe -p:Configuration=Release *.sln -maxcpucount $@ && \
MSBuild.exe INSTALL.vcxproj -p:Configuration=Release
