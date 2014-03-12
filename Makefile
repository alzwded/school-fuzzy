CPP = g++
CFLAGS = -g --std=c++03

a.out: fuzi.cpp fuzi.h functions.hpp
	$(CPP) $(CFLAGS) fuzi.cpp

clean:
	rm -f a.out
