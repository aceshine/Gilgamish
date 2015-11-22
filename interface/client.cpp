#include <iostream>

#include <pthread.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in their_addr;
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(8080);
	// their_addr.sin_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &their_addr.sin_addr);
	bzero(&(their_addr.sin_zero), 8);

	connect(sock_fd, (struct sockaddr*)(&their_addr), sizeof(their_addr));

	char msg[] = "hello world";
	char buf[1024] = { 0 };
	//向服务器发送字符串msg
	if (send(sock_fd, msg, strlen(msg), 0) ==- 1) {
		perror("send");
		exit(1);
	}
	//接受从服务器返回的信息
	int numbytes;
	if ((numbytes = recv(sock_fd, buf, 100, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	// buf[numbytes] = '/0';
	printf("result:%s",buf);
	close(sock_fd);
	return 0;
}
