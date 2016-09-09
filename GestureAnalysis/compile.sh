#!/bin/bash

#Remove the folder build
echo "Removing folder build"
rm -r build/

echo "Creating folder build"
#Create the dir build to compile
mkdir build

echo "Moving to folder build"
#Move to the build folder
cd build

echo "Configuring ..."
#Run cmake .. to configure
cmake ..

echo "Compiling ..."
#Compile using make
make

echo "Completed"