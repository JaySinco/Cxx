#!/bin/bash

source ../set_env.sh
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
