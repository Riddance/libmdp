#!/usr/bin/env bash

set -x
set -e

if [ ! -d "libzmq" ]; then
    git clone https://github.com/zeromq/libzmq
    (
        cd libzmq
        mkdir cmake-build
        cd cmake-build
        cmake ..
        make -j 4
    )
fi

mkdir build
cd build
cmake ../
make
