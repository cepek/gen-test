#!/bin/bash

DAT=`date -I`
rm -rf $DAT
mkdir  $DAT

ALG=svd
GEN=$DAT/gen3

ulimit -t 3

# rm -f ttt* test-* gen3-* tst3-*

if [ ! -f ./gama-local ]; then
    echo "Symlink ./gama-local does not exists ..."
    exit 1
fi

while (true)
do
    let i=i+1
    echo
    ./gen-test $GEN-$i-a.gkf $GEN-$i-b.gkf

    ./gama-local $GEN-$i-a.gkf --xml $GEN-$i-a.xml --text $GEN-$i-a.txt \
		 --export $GEN-$i-c.gkf --algorithm $ALG ;
    A=$?

    ./gama-local $GEN-$i-b.gkf --xml $GEN-$i-b.xml --text $GEN-$i-b.txt \
	 --algorithm gso
    B=$?

    ./check_xml_xml "xml diff $GEN-$i-a/b.xml ... exact corr. b" \
		       $GEN-$i-a.xml $GEN-$i-b.xml
    D=$?

    ./gama-local $GEN-$i-c.gkf --xml $GEN-$i-c.xml --text $GEN-$i-c.txt \
		    --algorithm $ALG
    C=$?

    ./check_xml_xml "xml diff $GEN-$i-b/c.xml ... export from a" \
		       $GEN-$i-b.xml $GEN-$i-c.xml
    E=$?

    if [ $A -ne 0 ] || \
       [ $B -ne 0 ] || \
       [ $C -ne 0 ] || \
       [ $D -ne 0 ] || \
       [ $E -ne 0 ]
    then
	echo
	echo gen-test a.gkf b.gkf
	echo
	echo A : gama-local a.gkf --xml a.xml --text a.txt \
	     --export c.gkf --algorithm $ALG
	echo B : gama-local b.gkf --xml b.xml --text b.txt --algorithm gso
	echo C : gama-local c.gkf --xml c.xml --text c.txt --algorithm $ALG
	echo D : check_xml_xml "diff a/b exact correction" a.xml b.xml
	echo E : check_xml_xml "diff b/c export from a   " b.xml c.xml
	echo
	echo tests A B C D E : $A $B $C $D $E
    	exit 1
    fi

    if [ $i -ge 100 ] ;
    then
        echo
        exit 0
    fi
done
