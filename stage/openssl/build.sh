#!/bin/bash

source ../set_env.sh
cd openssl-OpenSSL_1_1_1g
./config zlib \
    --with-zlib-include=${external_dir}/zlib/include/ \
    --with-zlib-lib=${external_dir}/zlib/lib/ \
    --openssldir=${external_dir}/openssl/ \
    --prefix=${external_dir}/openssl/ \
    no-asm \
&& \
$build && $install