#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build
cmake -G "$generator" ../curl-7.66.0 \
    -DOPENSSL_ROOT_DIR="${external_dir}/openssl/" \
    -DOPENSSL_CRYPTO_LIBRARY="${external_dir}/openssl/lib/libcrypto.a" \
    -DOPENSSL_INCLUDE_DIR="${external_dir}/openssl/include/" \
    -DZLIB_ROOT="${external_dir}/zlib/" \
    -DZLIB_INCLUDE_DIR="${external_dir}/zlib/include/" \
    -DZLIB_LIBRARY="${external_dir}/zlib/lib/libz.a" \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/curl/" \
    -DBUILD_SHARED_LIBS=off \
    -DCMAKE_USE_OPENSSL=on \
&& \
$build && $install
