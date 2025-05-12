#!/bin/bash
# This script is used to build the RISC-V simulator project

# Create a build directory
mkdir -p build
cd build
cmake ..
make

# Optionally, you can add a command to run your simulator after building
# ./simulator
