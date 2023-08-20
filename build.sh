#!/bin/bash

# This script tries to build the project.
cd ./build
qmake ../
bear --output ../compile_commands.json -- make debug

# bear --output ../compile_commands.json -- make release
# ../bin/hikeye
