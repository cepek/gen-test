#!/bin/bash

ulimit -t 3

rm -f ttt* test-*

if [ ! -f ./gama-local ]; then
    echo "Symlink ./gama-local does not exists ..."
    exit 1
fi

while (true)
do
    let i=i+1
    echo -n "$i "
    ./gen-test > ttt-$i.gkf

    if ! ./gama-local ttt-$i.gkf --text ttt-$i.txt --algorithm svd ;
    then
        if ! grep min_subset_x                ttt-$i.gkf &&
	   ! grep "No points available"       ttt-$i.gkf &&
	   ! grep "No observations available" ttt-$i.gkf ;
        then
            # echo "   " exception
            cp ttt-$i.gkf test-$i.gkf
            # cp ttt-$i.txt test-$i.txt
        fi;
    fi
#   rm -f ttt*

    if [ $i -ge 100 ] ;
    then
        echo
        exit 0
    fi
done
