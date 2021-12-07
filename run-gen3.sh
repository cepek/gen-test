#!/bin/bash

ulimit -t 3

rm -f ttt* test-* gen3-* tst3-*

if [ ! -f ./gama-local ]; then
    echo "Symlink ./gama-local does not exists ..."
    exit 1
fi

while (true)
do
    let i=i+1
    echo -n "$i "
    ./gen-test gen3-$i-a.gkf gen3-$i-b.gkf

    if ! ./gama-local gen3-$i-a.gkf --text gen3-$i-a.txt --algorithm svd ;
    then
        if ! grep min_subset_x                gen3-$i-a.gkf &&
	   ! grep "No points available"       gen3-$i-a.gkf &&
	   ! grep "No observations available" gen3-$i-a.gkf ;
        then
            # echo "   " exception
            cp gen3-$i-a.gkf tst3-$i-a.gkf
            # cp gen3-$i.txt tst3-$i.txt
        fi;
    fi
    if ! ./gama-local gen3-$i-b.gkf --text gen3-$i-b.txt --algorithm svd ;
    then
        if ! grep min_subset_x                gen3-$i-b.gkf &&
	   ! grep "No points available"       gen3-$i-b.gkf &&
	   ! grep "No observations available" gen3-$i-b.gkf ;
        then
            # echo "   " exception
            cp gen3-$i-b.gkf tst3-$i-b.gkf
            # cp gen3-$i.txt tst3-$i.txt
        fi;
    fi

    diff gen3-$i-a.txt gen3-$i-b.txt

    if [ $i -ge 100 ] ;
    then
        echo
        exit 0
    fi
done
