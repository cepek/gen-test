

gen-test : gen-test.o
	g++ -o gen-test gen-test.o

gen-test.o : gen-test.cpp Makefile
	g++ -c -Wall -pedantic gen-test.cpp


clean:
	rm -f ttt.* *svd *gso
	rm -f *.o
	rm -f `find . -name *[\~\#]*`

archive:
	tar czvf gen-test-`date -I`.tgz \
		gen-test.cpp run-tests run-gkf Makefile README
