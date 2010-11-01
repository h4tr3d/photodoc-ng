#!/bin/bash

cppcheck -v --template "gcc" --enable=all $@ *.cpp md5/*.c gphoto2/*.cpp
