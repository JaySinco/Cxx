#!/bin/bash

source ../set_env.sh

mkdir -p ${root_dir}/external/tinyobjloader/include/
cp -r tinyobjloader-1.0.6/*.h ${root_dir}/external/tinyobjloader/include/
mkdir -p ${root_dir}/external/tinyobjloader/src/
cp -r tinyobjloader-1.0.6/*.cc ${root_dir}/external/tinyobjloader/src/