#!/bin/bash

source project.conf

cd ..
rm -rf build-win32 > /dev/null 2>&1
mkdir build-win32
cd build-win32
#qmake -spec win32-cross-g++ image-ruler.pro
i486-mingw32-qmake ../$project.pro

echo "*** Now you can type 'cd .. && make' for build"
