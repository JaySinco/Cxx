_dir=`dirname $(readlink -f $0)`
current_dir=`readlink -f $_dir`

function find_root() {
    _pwd=`readlink -f "${1}"`
    _dep="${2}"
    >&2 echo "$_dep: $_pwd"
    sleep 0.1
    if [ -f "$_pwd/.root" ]
    then 
        readlink -f $_pwd
    else 
        find_root "$_pwd/../" $(expr $depth + 1)
    fi
}

root_dir=`find_root "${current_dir}" 0`
relative_dir=`realpath --relative-to "$root_dir" "$current_dir"`

echo
echo "##############################################################"
echo "#"
echo "#"
echo "#   USING current path: $current_dir"
echo "#   USING root: $root_dir"
echo "#   USING current relative path: $relative_dir"
echo "#"
echo "#"
echo "##############################################################"
echo
sleep 0.2

MSVC_PATH="/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/HostX86/x86:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/VC/VCPackages:/c/Program Files (x86)/Microsoft SDKs/TypeScript/3.1:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TestWindow:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/bin/Roslyn:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Team Tools/Performance Tools:/c/Program Files (x86)/Microsoft Visual Studio/Shared/Common/VSPerfCollectionTools/:/c/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.6.1 Tools/:/c/Program Files (x86)/Windows Kits/10/bin/10.0.17763.0/x86:/c/Program Files (x86)/Windows Kits/10/bin/x86:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community//MSBuild/15.0/bin:/c/Windows/Microsoft.NET/Framework/v4.0.30319:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/:/c/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/Tools/"
PATH="${MSVC_PATH};${PATH}"

pushd "$root_dir"
mkdir -p dest/ &&\
pushd dest/ && \
# -T "LLVM"
cmake -G "Visual Studio 15 2017 Win64" ../ \
-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=${current_dir}/bin/ \
-DMSVC_RUNTIME=dynamic \
&& \
popd  &&\
MSBuild.exe -p:Configuration=Release dest/$relative_dir/__target*.vcxproj -maxcpucount $@ && \
popd
