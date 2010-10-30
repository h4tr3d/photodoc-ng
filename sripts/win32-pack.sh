#!/bin/bash

source project.conf

cd ..
VERSION=`cat src/config.h | grep '^#define' | grep 'APP_VERSION' | awk '{print $3}' | sed 's/"//g'`

[ -z "$VERSION" ] && VERSION="$project_default_version"

target=$project-$VERSION-win32
rm -rf $target > /dev/null 2>&1
mkdir $target
cp -a build-win32/release/$project.exe $additional_files $target
cp $MINGW32_DLL $target
cp $DLL $target

for lib in $QT_DLL
do
  cp $QT_LIB_PREFIX/$lib $target
done

cwd=`pwd`
zip -r ../$target.zip $target
