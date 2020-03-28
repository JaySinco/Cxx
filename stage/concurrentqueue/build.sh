#!/bin/bash

source ../set_env.sh
mkdir -p ${root_dir}/external/windows/x64/concurrentqueue/include/
cp -r concurrentqueue-1.0.1/*.h ${root_dir}/external/windows/x64/concurrentqueue/include/