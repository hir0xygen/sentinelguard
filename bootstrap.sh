#!/bin/bash

rm -r sentinel-build
mkdir -p sentinel-build && cd sentinel-build || exit && cmake .. && make && cd .. && sudo cp sentinel-build/sentinelguard /usr/local/bin/sentinelguard && sudo chmod +s /usr/local/bin/sentinelguard && rm -r sentinel-build
