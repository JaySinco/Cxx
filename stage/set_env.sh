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
nproc=`nproc`
build="make -j $nproc"
install="make install"
external_dir="${root_dir}/external/linux/x64"
generator="Unix Makefiles"
