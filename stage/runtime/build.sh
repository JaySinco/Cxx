#!/bin/bash

# cn_visual_cpp_redistributable_for_visual_studio_2017_x64.exe
# w_mkl_2020.0.166.exe

ls -l --ignore=build.sh | awk 'NR == 1 {next} {print "#", $9}'