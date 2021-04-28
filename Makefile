CXX = g++
FLAGS = -std=c++20 -O2 -g -I . -I include -Wall -Wextra -pedantic

default:
	$(CXX) $(FLAGS) -c src/Example.cpp
	ar rcs libforklift.a *.o
	$(CXX) $(FLAGS) -c main.cpp
	$(CXX) -L . -o main main.o -lforklift -lsqlite3
	rm -f *.o

