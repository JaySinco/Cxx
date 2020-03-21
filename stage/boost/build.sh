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

cd boost_1_71_0

# ./bootstrap.bat
./b2.exe --with-system --with-thread --with-filesystem --with-chrono\
    --with-program_options --with-test --with-timer --with-atomic \
    --prefix=${root_dir}/external/windows/x64/boost/ \
    toolset=msvc variant=release link=static \
    architecture=x86 address-model=64 threading=multi \
    runtime-link=shared && \
mkdir -p ${root_dir}/external/windows/x64/boost/include/ && \
mkdir -p ${root_dir}/external/windows/x64/boost/lib/ && \
cp -r stage/lib/*.lib ${root_dir}/external/windows/x64/boost/lib/