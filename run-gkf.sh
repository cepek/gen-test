#!/bin/bash

ulimit -t 300

for i in *.gkf
do
    echo   $i
    ./gama $i
done
