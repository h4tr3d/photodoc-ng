#!/bin/bash

cppcheck -v --template "gcc" --enable=all $@ ../src/*.cpp ../src/md5/*.c ../src/gphoto2/*.cpp
