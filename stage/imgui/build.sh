#!/bin/bash

source ../set_env.sh

mkdir -p ${root_dir}/external/imgui/include/
cp -r imgui-1.72b/*.h ${root_dir}/external/imgui/include/
cp -r imgui-1.72b/examples/*.h ${root_dir}/external/imgui/include/
mkdir -p ${root_dir}/external/imgui/src/
cp -r imgui-1.72b/*.cpp ${root_dir}/external/imgui/src/
cp -r imgui-1.72b/examples/*.cpp ${root_dir}/external/imgui/src/