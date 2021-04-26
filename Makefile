CXX = g++
FLAGS = -O2 -g -I include -Wall -Wextra -pedantic

default:
	$(CXX) $(FLAGS) -c src/Example.cpp
	ar rcs libforklift.a *.o
	rm -f *.o

