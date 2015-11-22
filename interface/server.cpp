#include <iostream>

#include <pthread.h>
#include <unistd.h>

#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static void* thread_function(void* args)
{
	std::cout << "thread_function" << std::endl;
	std::string test(static_cast<char*>(args));
	printf("args is %s\n", (char*)args);
	// pthread_exit((void*)0);
	pthread_exit(NULL);
	return NULL;
}

static void* server_thread(void* args)
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	int flags = fcntl(sock_fd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(sock_fd, F_SETFL, flags);

	struct sockaddr_in server_addr;
	// memset(server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

	bind(sock_fd, (struct sockaddr*)(&server_addr), sizeof(server_addr));

	listen(sock_fd, 1024);

	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	int fd_A[1024] = { 0 };
	int conn_amount = 0;
	fd_set fdsr;
	int maxsock = sock_fd;
	FD_ZERO(&fdsr);
	FD_SET(sock_fd, &fdsr);
	
	for (;;)
	{
		int ret = select(maxsock + 1, &fdsr, NULL, NULL, &timeout);
		if (ret < 0)
			break;
		else if (ret == 0)
			continue;

		for (int i = 0; i < conn_amount; i++)
		{
			if (FD_ISSET(fd_A[i], &fdsr))
			{
				char buf[1024] = { 0 };
				char str[] = "Good,very nice!\n";
				int ret = recv(fd_A[i], buf, sizeof(buf)/sizeof(char), 0);
				send(fd_A[i], str, sizeof(str)/sizeof(char)+1, 0);
				if (ret <= 0)
				{
					close(fd_A[i]);
					FD_CLR(fd_A[i], &fdsr);
					fd_A[i] = 0;
					conn_amount -= 1;
				}
			}
		}

		if (FD_ISSET(sock_fd, &fdsr))
		{
			struct sockaddr_in client_addr;
			socklen_t s = sizeof(client_addr);
			int client = accept(sock_fd, (struct sockaddr*)(&client_addr), &s);
			if (conn_amount < 1024)
			{
				conn_amount += 1;
				fd_A[conn_amount] = client;
				FD_SET(fd_A[conn_amount], &fdsr);
			}
		}
	}

	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char* argv[])
{
	std::cout << "server main" << std::endl;

	pthread_t tid;
	void* thread_result;

	char args[] = "test";
	pthread_create(&tid, NULL, thread_function, (void*)args);
	// sleep(1);
	pthread_join(tid, &thread_result);
	if (thread_result == NULL)
		printf("thread over\n");
	// printf("thread result is %d\n", *((int*)thread_result));

	pthread_create(&tid, NULL, server_thread, NULL);
	pthread_join(tid, &thread_result);
	if (thread_result == NULL)
		printf("server over\n");

	printf("server over\n");
	return 0;
}
