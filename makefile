all: test

test: barec_naked.c
	gcc -Wall barec_naked.c -o barec_naked

barec_naked.c: barec
	./barec keephash < barec.c > barec_naked.c

barec: barec.c
	gcc -g -Wall barec.c -o barec

clean:
	rm barec barec_naked barec_naked.c

