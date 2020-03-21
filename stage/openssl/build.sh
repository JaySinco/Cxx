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

cd openssl-OpenSSL_1_1_1e

perl Configure VC-WIN64A zlib \
    --with-zlib-include=${root_dir}/external/windows/x64/zlib/include/ \
    --with-zlib-lib=${root_dir}/external/windows/x64/zlib/lib/zlibstatic.lib \
    --openssldir=${root_dir}/external/windows/x64/openssl/ \
    --prefix=${root_dir}/external/windows/x64/openssl/
# 以下需要在VS本机工具命令行中执行
# nmake
# nmake install