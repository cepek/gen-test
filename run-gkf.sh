#!/bin/bash

ulimit -t 300

if [ ! -f ./gama-local ]; then
    echo "Symlink ./gama-local does not exists ..."
    exit 1
fi

for i in *.gkf
do
    echo   $i
    ./gama-local $i
done
