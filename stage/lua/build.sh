#!/bin/bash

source ../set_env.sh

mkdir -p ${root_dir}/external/lua/include/
cp -r lua-5.3.5/src/*.h ${root_dir}/external/lua/include/
mkdir -p ${root_dir}/external/lua/src/
cp -r lua-5.3.5/src/*.c ${root_dir}/external/lua/src/