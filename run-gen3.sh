#!/bin/bash

ALG=svd

ulimit -t 3

rm -f ttt* test-* gen3-* tst3-*

if [ ! -f ./gama-local ]; then
    echo "Symlink ./gama-local does not exists ..."
    exit 1
fi

while (true)
do
    let i=i+1
    # echo -n "$i "
    ./gen-test gen3-$i-a.gkf gen3-$i-b.gkf

    if ! ./gama-local gen3-$i-a.gkf --xml gen3-$i-a.xml --text gen3-$i-a.txt \
            --export gen3-$i-c.gkf --algorithm $ALG ;
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
    if ! ./gama-local gen3-$i-b.gkf --xml gen3-$i-b.xml --text gen3-$i-b.txt \
	 --algorithm $ALG;
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

    # diff gen3-1-a.gkf gen3-1-b.gkf

    echo
    ./check_xml_xml "xml diff gen3-$i-a/b.xml ... exact corrections b" \
		    gen3-$i-a.xml gen3-$i-b.xml

    ./gama-local gen3-$i-c.gkf --xml gen3-$i-c.xml --text gen3-$i-c.txt \
		 --algorithm $ALG ;

    ./check_xml_xml "xml diff gen3-$i-a/c.xml ... exported data from a" \
		    gen3-$i-b.xml gen3-$i-c.xml

    if [ $i -ge 100 ] ;
    then
        echo
        exit 0
    fi
done
