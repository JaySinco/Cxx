#!/bin/bash

source ../set_env.sh
mkdir -p ${root_dir}/external/windows/x64/json/include/
cp -r json-3.7.3-include/single_include/nlohmann ${root_dir}/external/windows/x64/json/include/