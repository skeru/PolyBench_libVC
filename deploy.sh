#!/bin/bash

rm -rf libVersioningCompiler/build
mkdir libVersioningCompiler/build
cd libVersioningCompiler/build
cmake .. -DCMAKE_INSTALL_PREFIX="."
make install
cd ../../

rm -rf cmake
mkdir cmake
cp libVersioningCompiler/config/FindLibVersioningCompiler.cmake cmake/

rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="."
make
