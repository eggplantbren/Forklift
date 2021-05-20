CXX = g++
FLAGS = -std=c++20 -O3 -march=native -I . -I include -Wall -Wextra -pedantic

default:
	$(CXX) $(FLAGS) -c src/Config.cpp
	$(CXX) $(FLAGS) -c src/Database.cpp
	$(CXX) $(FLAGS) -c src/Double.cpp
	ar rcs libforklift.a *.o
	$(CXX) $(FLAGS) -c main.cpp
	$(CXX) -L . -o main main.o -lforklift -lsqlite3 -lyaml-cpp
	rm -f *.o

