flags = -g -lstdc++ -std=c++11

all: sort

sort: sort.o
	gcc $(flags) -o sort sort.o

sort.o: sort.cpp
	gcc $(flags) -o sort.o -c sort.cpp

clean:
	rm -f ./sort ./*.o
