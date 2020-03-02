#!/bin/bash

_dir=`dirname $(readlink -f $0)`
MSVC_PATH="/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/HostX86/x86:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/VC/VCPackages:/c/Program Files (x86)/Microsoft SDKs/TypeScript/3.1:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TestWindow:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/bin/Roslyn:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Team Tools/Performance Tools:/c/Program Files (x86)/Microsoft Visual Studio/Shared/Common/VSPerfCollectionTools/:/c/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.6.1 Tools/:/c/Program Files (x86)/Windows Kits/10/bin/10.0.17763.0/x86:/c/Program Files (x86)/Windows Kits/10/bin/x86:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community//MSBuild/15.0/bin:/c/Windows/Microsoft.NET/Framework/v4.0.30319:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/Tools/"
PATH="${MSVC_PATH};${PATH}"
mkdir -p cmake-build
cd cmake-build
cmake -G "Visual Studio 15 2017 Win64" -T "host=x64"  ../ \
-DCMAKE_CXX_STANDARD=11 \
-DBUILD_SHARED_LIBS=OFF \
-DCMAKE_CXX_FLAGS="/Zc:__cplusplus" \
-DCMAKE_INSTALL_PREFIX="./mongo-cxx-driver" \
-DCMAKE_PREFIX_PATH=`readlink -f ../../mongo-c-driver-1.13.0/cmake-build/mongo-c-driver/` \
-DBOOST_ROOT=`readlink -f ../../../windows/x64/boost/` \
-DBUILD_VERSION="r3.4.0" && \
MSBuild.exe -p:Configuration=Release MONGO_CXX_DRIVER.sln -maxcpucount $@
MSBuild.exe INSTALL.vcxproj -p:Configuration=Release