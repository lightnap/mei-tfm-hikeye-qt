#!/usr/bin/env bash

# This script tries to build the project.
#TODO: HK-41 Add help and description of flags.

# delete build directory if given the -c flag.
if [[ $* == *-c* ]]
then
    rm -r "./build"
fi

# Create build folder if it doesnt exists.
if [ ! -d "./build" ] 
then
    mkdir build
fi

# Build the project. 
cd ./build
#cmake -DCMAKE_BUILD_TYPE=Release ../
cmake -DCMAKE_BUILD_TYPE=Debug ../
cmake --build . -j 8

#bear --output ../compile_commands.json -- make debug -j 6
# bear --output ../compile_commands.json -- make release
# ../bin/hikeye
