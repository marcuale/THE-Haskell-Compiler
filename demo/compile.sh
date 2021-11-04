#!/bin/bash

if python3 codegen.py $1 codegen/target.c; then
    cd codegen
    gcc -Wall -Werror default.c target.c gc.c
    cp a.out ..
    cd ..
fi

