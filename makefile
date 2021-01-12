CFLAGS= -Wall -g

all:tree

tree:tree.o
	gcc $(CFLAGS) tree.o  -o frequency

tree.o:tree.c
	gcc $(CFLAGS) -c tree.c


.PHONY: clean all

clean:
	rm -f *.o frequency 
