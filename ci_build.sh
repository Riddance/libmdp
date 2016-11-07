#!/usr/bin/env bash

set -x
set -e

git clone https://github.com/zeromq/libzmq
(
    mkdir cmake-build && cd cmake-build
    cmake .. && make -j 4
    make test && make install && sudo ldconfig
)

mkdir build
cd build
cmake ../
make
