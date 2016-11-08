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
        sudo make install
        sudo ldconfig
        cd ..
    )
fi

mkdir build
cd build
cmake ../
make
