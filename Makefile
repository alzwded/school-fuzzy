CPP = g++
CFLAGS = -g --std=c++03 -Wall -pedantic-errors
EXTRACFLAGS =

a.out: fuzi.cpp fuzification.hxx fuzi.hxx functions.hpp multidimarray.hxx
	$(CPP) $(CFLAGS) $(EXTRACFLAGS) fuzi.cpp

clean:
	rm -f a.out
