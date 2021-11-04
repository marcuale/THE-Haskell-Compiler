#!/bin/sh

python3 main.py $1
cp $1.c codegen/target.c
cd codegen
gcc target.c default.c gc.c
cp a.out ../$1.out
cd ..
