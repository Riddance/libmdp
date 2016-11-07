#!/usr/bin/env bash

set -x
set -e

git clone --depth 1 -b stable git@github.com:zeromq/libzmq.git
(
    cd libzmq
    ./autogen.sh
    ./configure
    sudo make install
    sudo ldconfig
)

mkdir build
cd build
cmake ../
make
