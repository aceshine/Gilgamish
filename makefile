flags = -g -lstdc++ -std=c++11

all: main libstatic

main: main.o libstatic libdynamic
	$(MAKE) -C ./algorithm
	#gcc $(flags) -o main main.o libstatic.a
	gcc $(flags) -o main main.o -L. -lstatic -ldynamic

main.o: main.cpp main.hpp
	gcc $(flags) -o main.o -c main.cpp

libstatic: static.o
	ar rcs libstatic.a static.o

static.o: static.cpp
	gcc $(flags) -o static.o -c static.cpp

libdynamic: dynamic.o
	gcc $(flags) -shared -o libdynamic.so dynamic.o

dynamic.o: dynamic.cpp
	gcc $(flags) -fPIC -o dynamic.o -c dynamic.cpp

clean:
	rm -f ./main ./libstatic.a ./libdynamic.so ./*.o
	$(MAKE) -C ./algorithm clean
