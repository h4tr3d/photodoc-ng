#!/bin/bash

[ "$1" == "-g" ] && ADD="--db-attach=yes"

valgrind -v --track-fds=yes \
            --trace-children=yes \
            --track-origins=yes \
            --undef-value-errors=yes  \
            --leak-check=full \
            --leak-resolution=high $ADD \
            ../../photodoc-build-desktop/photodoc 2>&1 | tee valgrind.txt
#            ./labirint
