#!/bin/bash

rm -r sentinel-build
mkdir -p sentinel-build
cd sentinel-build || exit
cmake ..
make
cd ..
cp sentinel-build/sentinelguard sentinelguard
rm -r sentinel-build