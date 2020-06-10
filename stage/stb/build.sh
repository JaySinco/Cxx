#!/bin/bash

source ../set_env.sh
mkdir -p ${external_dir}/stb/include/
cp -r stb-master/*.h ${external_dir}/stb/include/