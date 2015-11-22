#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>

int main(int argc, char* argv[])
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = 8080;
	inet_pton(AF_INET, "127.0.0.1", (void*)&addr.sin_addr);
	printf("%d\n", addr.sin_addr.s_addr);
	return 0;
}
