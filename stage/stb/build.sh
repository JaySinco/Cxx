#!/bin/bash

source ../set_env.sh
mkdir -p ${root_dir}/external/windows/x64/stb/include/
cp -r stb-master/*.h ${root_dir}/external/windows/x64/stb/include/