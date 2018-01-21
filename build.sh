#!/bin/bash
current_path=$PWD
handler_name=$1
binary_name=$2
package_name=$3
temp=`mktemp -d`

mkdir $temp/build
mkdir $temp/$handler_name

cd $temp/build
cmake /usr/local/app
make

cp $temp/build/$binary_name $temp/$handler_name.so

rm -rf $temp/build

cp /usr/local/runtime/dist/__init__.pyc $temp/$handler_name/__init__.pyc
cp /usr/local/runtime/dist/proxy.pyc $temp/$handler_name/proxy.pyc
cp /usr/local/runtime/dist/libruntime.so $temp/$handler_name/runtime.so

mkdir $temp/lib

cp /usr/lib64/libstdc++* $temp/lib/

cd $temp
find . -exec touch -t 201302210800 {} +
zip -qrX $package_name *

mv $package_name $current_path/.

rm -rf $temp