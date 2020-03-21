#!/bin/bash

source ../set_env.sh

mkdir -p ${root_dir}/external/tinyxml/include/
cp -r tinyxml2-7.1.0/*.h ${root_dir}/external/tinyxml/include/
mkdir -p ${root_dir}/external/tinyxml/src/
cp -r tinyxml2-7.1.0/*.cpp ${root_dir}/external/tinyxml/src/