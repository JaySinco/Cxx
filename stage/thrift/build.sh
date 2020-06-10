#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build
sudo apt install flex bison
cmake -G "$generator" ../thrift-0.12.0 \
    -DLIBEVENT_ROOT="${external_dir}/libevent/" \
    -DLIBEVENT_INCLUDE_DIRS="${external_dir}/libevent/include/" \
    -DLIBEVENT_LIBRARIES="${external_dir}/libevent/lib/"  \
    -DOPENSSL_ROOT_DIR="${external_dir}/openssl/" \
    -DOPENSSL_CRYPTO_LIBRARY="${external_dir}/openssl/lib/libcrypto.a" \
    -DOPENSSL_INCLUDE_DIR="${external_dir}/openssl/include/" \
    -DBOOST_ROOT="${external_dir}/boost/" \
    -DBOOST_LIBRARYDIR="${external_dir}/boost/lib/" \
    -DZLIB_ROOT="${external_dir}/zlib/" \
    -DZLIB_INCLUDE_DIR="${external_dir}/zlib/include/" \
    -DZLIB_LIBRARY="${external_dir}/zlib/lib/libz.a" \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/thrift" \
    -DWITH_SHARED_LIB=off \
    -DWITH_STATIC_LIB=on \
    -DWITH_STDTHREADS=on \
    -DWITH_BOOSTTHREADS=off \
    -DWITH_BOOST_STATIC=on \
    -DWITH_ZLIB=on \
    -DWITH_LIBEVENT=on \
    -DBUILD_TESTING=off \
&& \
$build && $install