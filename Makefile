CPP = g++
CFLAGS = -g --std=c++03 -pedantic-errors

a.out: fuzi.cpp fuzification.hxx fuzi.hxx functions.hpp
	$(CPP) $(CFLAGS) fuzi.cpp

clean:
	rm -f a.out
