#!/bin/bash

source ../set_env.sh
mkdir -p ${root_dir}/external/windows/x64/pprint/include/pprint/
cp -r pprint-0.9.1/include/* ${root_dir}/external/windows/x64/pprint/include/pprint/