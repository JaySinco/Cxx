#!/bin/bash

source ../set_env.sh
cd openssl-OpenSSL_1_1_1e

perl Configure VC-WIN64A zlib \
    --with-zlib-include=${root_dir}/external/windows/x64/zlib/include/ \
    --with-zlib-lib=${root_dir}/external/windows/x64/zlib/lib/zlibstatic.lib \
    --openssldir=${root_dir}/external/windows/x64/openssl/ \
    --prefix=${root_dir}/external/windows/x64/openssl/
# 以下需要在VS本机工具命令行中执行
# nmake
# nmake install