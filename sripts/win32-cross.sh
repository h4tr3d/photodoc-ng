#!/bin/bash

source project.conf

cd ..
#tmpf1=`mktemp`
#tmpf2=`mktemp`
#cat $project.pro | grep '^CONFIG' | tee $tmpf1 | sed 's/debug/release/' > $tmpf2

#line1=`cat $tmpf1`
#line2=`cat $tmpf2`

#cat $project.pro | sed "s/$line1/$line2/" > $project-win32.pro
#mv $project-win32.pro $project.pro

#qmake -spec win32-cross-g++ image-ruler.pro
rm -rf build-win32 > /dev/null 2>&1
mkdir build-win32
cd build-win32
i486-mingw32-qmake ../$project.pro

echo "*** Now you can type 'cd .. && make' for build"
