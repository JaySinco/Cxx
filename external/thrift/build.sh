#!/bin/bash

_dir=`dirname $(readlink -f $0)`
MSVC_PATH="/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/HostX86/x86:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/VC/VCPackages:/c/Program Files (x86)/Microsoft SDKs/TypeScript/3.1:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TestWindow:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/bin/Roslyn:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Team Tools/Performance Tools:/c/Program Files (x86)/Microsoft Visual Studio/Shared/Common/VSPerfCollectionTools/:/c/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.6.1 Tools/:/c/Program Files (x86)/Windows Kits/10/bin/10.0.17763.0/x86:/c/Program Files (x86)/Windows Kits/10/bin/x86:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community//MSBuild/15.0/bin:/c/Windows/Microsoft.NET/Framework/v4.0.30319:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/Tools/"
PATH="${MSVC_PATH};${PATH}"

cmake -G "Visual Studio 15 2017 Win64" -T "host=x64"  ../ \
-DLIBEVENT_ROOT=`readlink -f ../../../windows/x64/libevent/` \
-DLIBEVENT_INCLUDE_DIRS=`readlink -f ../../../windows/x64/libevent/include/` \
-DLIBEVENT_LIBRARIES=`readlink -f ../../../windows/x64/libevent/lib/` \
-DOPENSSL_ROOT_DIR=`readlink -f ../../../windows/x64/openssl/` \
-DOPENSSL_CRYPTO_LIBRARY=`readlink -f ../../../windows/x64/openssl/lib/` \
-DOPENSSL_INCLUDE_DIR=`readlink -f ../../../windows/x64/openssl/include/` \
-DBOOST_ROOT=`readlink -f ../../../windows/x64/boost/` \
-DBOOST_LIBRARYDIR=`readlink -f ../../../windows/x64/boost/lib/` \
-DBoost_USE_STATIC_RUNTIME=on \
-DBoost_COMPILER="-vc141" \
-DZLIB_ROOT=`readlink -f ../../../zlib/zlib-1.2.11/` \
-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=$_dir/bin/ \
-DWITH_SHARED_LIB=off \
-DWITH_STATIC_LIB=on \
-DWITH_STDTHREADS=on \
-DWITH_BOOSTTHREADS=off \
-DWITH_BOOST_STATIC=on \
-DWITH_ZLIB=on \
-DWITH_LIBEVENT=on \
-DBUILD_TESTING=off \
-DMSVC_RUNTIME=dynamic && \
MSBuild.exe -p:Configuration=Release *.sln $@
