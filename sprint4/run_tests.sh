#!/bin/bash

for t in tests/*.thc; do
    echo Running test $t
    head -n1 $t
    if python3 codegen.py $t codegen/target.c; then
        cd codegen
        gcc -Wall -Werror default.c target.c gc.c
        timeout 5 ./a.out || echo Timeout
        cd ..
    fi
    echo
    echo
done

