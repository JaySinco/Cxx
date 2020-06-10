#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build
cmake -G "$generator" ../libevent-release-2.1.11-stable \
    -DOPENSSL_ROOT_DIR="${external_dir}/openssl/" \
    -DOPENSSL_CRYPTO_LIBRARY="${external_dir}/openssl/lib/libcrypto.so.1.1" \
    -DOPENSSL_INCLUDE_DIR="${external_dir}/openssl/include/" \
    -DCMAKE_INSTALL_PREFIX="${external_dir}/libevent" \
    -DEVENT__LIBRARY_TYPE=static \
&& \
$build && $install
