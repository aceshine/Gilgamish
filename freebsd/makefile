cc = gcc
flags = -g -lstdc++ -std=c++11

kqueue: kqueue.o
	$(cc) $(flags) -o kqueue kqueue.o -lpthread

kqueue.o: kqueue.cpp
	$(cc) $(flags) -o kqueue.o -c kqueue.cpp

clean:
	rm -f kqueue ./*.o

run:
	./kqueue