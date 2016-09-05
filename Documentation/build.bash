#!/bin/bash

START_DIR_BUILD=$PWD

cd ..
cd ..

if [ ! -d "Datastructures" ]; then
echo Couldnt find script path
else
cd Datastructures
if [ ! -d "Unix" ]; then
echo Couldnt find script path
else
cd Unix
. ./buildDocumentation.bash CryptoLogin
fi
fi

cd $START_DIR_BUILD
