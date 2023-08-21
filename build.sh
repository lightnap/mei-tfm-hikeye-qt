#!/bin/bash

# This script tries to build the project.

# Create build folder if it doesnt exists.
if [ ! -d "./build" ] 
then
    mkdir build
fi

# Build the project. 
cd ./build
qmake ../
bear --output ../compile_commands.json -- make debug

# bear --output ../compile_commands.json -- make release
# ../bin/hikeye
