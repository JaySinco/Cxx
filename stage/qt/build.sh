#!/bin/bash

source ../set_env.sh
mkdir -p build
cd build

# 以下需要在vs2017 x64命令行中执行
# ..\qt-everywhere-src-5.12.7\configure -static -release -nomake examples -nomake tests -skip qtwebengine -opensource -confirm-license -prefix "D:\Jaysinco\Cxx\external\windows\x64\qt"
# jom
# jom install
