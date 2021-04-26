CXX = g++
FLAGS = -std=c++20 -O2 -g -I . -I include -Wall -Wextra -pedantic

default:
	$(CXX) $(FLAGS) -c src/Example.cpp
	$(CXX) $(FLAGS) -c include/Model.h
	ar rcs libforklift.a *.o
	rm -f *.o include/*.gch

