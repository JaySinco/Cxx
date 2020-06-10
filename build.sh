#!/bin/bash

current_dir=`readlink -f .`

function find_root() {
    _pwd=`readlink -f "${1}"`
    _dep="${2}"
    >&2 echo "$_dep: $_pwd"
    sleep 0.1
    if [ -f "$_pwd/.root" ]
    then 
        readlink -f $_pwd
    else 
        find_root "$_pwd/../" $(expr $_dep + 1)
    fi
}

root_dir=`find_root "${current_dir}" 0`
relative_dir=`realpath --relative-to "$root_dir" "$current_dir"`

# update ROOT_DIR
macro_str="#define ROOT_DIR \"${root_dir}\""
file_header=`head $root_dir/.root`
if [ "$file_header" != "$macro_str" ]
then
    echo $macro_str > $root_dir/.root
fi

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

cd $root_dir
mkdir -p dest/ &&\
cd dest/ && \
cmake -G "Unix Makefiles"  ../ \
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${current_dir}/bin/ \
&& \
cd $relative_dir && \
make -j `nproc`
