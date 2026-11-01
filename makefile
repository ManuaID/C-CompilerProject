CFLAGS=-std=c11 -g -static

9cc: 9cc.c

test: 9cc
	./test.sh

asm: tmp.s
	cc -o tmp tmp.s 
	./tmp; echo $$?

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean

