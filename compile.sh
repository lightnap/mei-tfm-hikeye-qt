#!/bin/bash

# This script tries to build the project.
#TODO: Add help and description of flags. 

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
qmake ../
bear --output ../compile_commands.json -- make debug -j 6

# bear --output ../compile_commands.json -- make release
# ../bin/hikeye
