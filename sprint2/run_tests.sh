#!/bin/bash

for t in tests/*.thc; do
    echo Running test $t
    python3 codegen.py $t > codegen/target.c
    head -n1 $t
    cd codegen
    gcc -Wall -Werror default.c target.c gc.c
    timeout 5 ./a.out || echo Timeout
    cd ..
    echo
    echo
done

