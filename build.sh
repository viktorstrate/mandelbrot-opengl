#!/bin/sh
set -e # exit on error

mkdir -p build/
cd build/
cmake ..

../generate_shaders.sh
make

echo "Compiled successfully, binary is located at ./build/mandlebrot"
