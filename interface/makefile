flags = -g -lstdc++ -std=c++11

all: server client

server: server.o
	gcc $(flags) -o server server.o -lpthread

server.o: server.cpp
	gcc $(flags) -o server.o -c server.cpp

client: client.o
	gcc $(flags) -o client client.o -lpthread

client.o: client.cpp
	gcc $(flags) -o client.o -c client.cpp

clean:
	rm -f ./server ./client ./*.o
