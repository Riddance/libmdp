#!/usr/bin/env bash

set -x
set -e

cd libzmq
mkdir cmake-build
cd cmake-build
cmake ..
sudo make install
sudo ldconfig
cd ..

mkdir build
cd build
cmake ../
make
