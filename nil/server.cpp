//
//  server.cpp
//  nil
//
//  Created by nil on 11/21/15.
//  Copyright © 2015 nil. All rights reserved.
//

#include "server.hpp"

#include <pthread.h>
#include <fcntl.h>
#include <poll.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/event.h>

#include <iostream>

static int server_init()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    return fd;
}

static void* normal_server(void* argv)
{
    std::cout << "main" << std::endl;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);
    if (int temp = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("bind error: %d \n", temp);
    }
    
    if (int temp = listen(server_fd, 1024) < 0)
    {
        printf("listen error: %d \n", temp);
    }
    
    for ( ;  ; ) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
        if (client_fd < 0) break;
        
        char buff[1024] = { 0 };
        ssize_t count = recv(client_fd, buff, sizeof(buff), 0);
        printf("server recv count %zd, %s \n", count, buff);
        
        count = send(client_fd, buff, sizeof(buff), 0);
        printf("server send count %zd, %s \n", count, buff);
        
        break;
    }
    
    pthread_exit(nullptr);
    return nullptr;
}

static void* select_server(void* argv)
{
    std::cout << "main" << std::endl;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    int flags = fcntl(server_fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(server_fd, F_SETFL, flags);
    
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    //    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);
    if (int temp = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("bind error: %d \n", temp);
    }
    
    if (int temp = listen(server_fd, 1024) < 0)
    {
        printf("listen error: %d \n", temp);
    }
    
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    
    int fd_array[1024] = { 0 };
    int max_count = 0;
    int max_fd = server_fd;
    fd_set fdsr;
    FD_ZERO(&fdsr);
    FD_SET(server_fd, &fdsr);
    
    for ( ;  ; ) {
        int ret = select(max_fd+1, &fdsr, nullptr, nullptr, &timeout);
        if (ret < 0)
            break;
        else if (ret == 0)
            continue;
        
        for (int i = 0; i < max_count; i++)
        {
            if (FD_ISSET(fd_array[i], &fdsr))
            {
                int client_fd = fd_array[i];
                char buff[1024] = { 0 };
                ssize_t count = recv(client_fd, buff, sizeof(buff), 0);
                printf("server recv count %zd, %s \n", count, buff);
                if (count <= 0)
                {
                    close(client_fd);
                    FD_CLR(fd_array[i], &fdsr);
                    fd_array[i] = 0;
                    max_count -= 1;
                    std::cout << "one client close" << std::endl;
                    goto over;
//                    continue;
                }
                count = send(client_fd, buff, sizeof(buff), 0);
                printf("server send count %zd, %s \n", count, buff);
            }
        }
        
        if (FD_ISSET(server_fd, &fdsr))
        {
            struct sockaddr_in client_addr;
            socklen_t len = sizeof(client_addr);
            int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
//            if (client_fd < 0) break;
            fd_array[max_count] = client_fd;
            FD_SET(fd_array[max_count], &fdsr);
            max_count += 1;
            if (client_fd > max_fd)
                max_fd = client_fd;

//            char buff[1024] = { 0 };
//            ssize_t count = recv(client_fd, buff, sizeof(buff), 0);
//            printf("select server recv count %zd, %s \n", count, buff);
//            
//            count = send(client_fd, buff, sizeof(buff), 0);
//            printf("select server send count %zd, %s \n", count, buff);

        }
//        struct sockaddr_in client_addr;
//        socklen_t len = sizeof(client_addr);
//        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
//        if (client_fd < 0) break;
//        
//        char buff[1024] = { 0 };
//        ssize_t count = recv(client_fd, buff, sizeof(buff), 0);
//        printf("server recv count %zd, %s \n", count, buff);
//        
//        count = send(client_fd, buff, sizeof(buff), 0);
//        printf("server send count %zd, %s \n", count, buff);
//
//        break;
    }
over:
    pthread_exit(nullptr);
    return nullptr;
}

static void* poll_server(void* argv)
{
    std::cout << "main" << std::endl;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    int flags = fcntl(server_fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(server_fd, F_SETFL, flags);
    
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    // inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);
    if (int temp = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("bind error: %d \n", temp);
    }
    
    if (int temp = listen(server_fd, 1024) < 0)
    {
        printf("listen error: %d \n", temp);
    }
    
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    
    struct pollfd client[OPEN_MAX];
    for (int i = 0; i < OPEN_MAX; i++) {
        client[i].fd = -1;
    }
    client[0].fd = server_fd;
    client[0].events = POLLRDNORM;
    int max_count = 0;
    for ( ;  ; ) {
        int ready = poll(client, max_count + 1, -1);
        if (client[0].revents & POLLRDNORM)
        {
            ready -= 1;
            if (ready <= 0) goto over;
        }
        for (int i = 0; i < max_count; i++) {
            
        }
    }
over:
    pthread_exit(nullptr);
    return nullptr;
}

static void* kqueue_server(void* argv)
{
	struct kevent change;
	struct kevent event;
	int kq = kqueue();
	EV_SET(&change, 1, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 5000, 0);
	for ( ; ; )
	{
		int nev = kevent(kq, &change, 1, &event, 1, NULL);
	}
    pthread_exit(nullptr);
    return nullptr;
}

void* server(void* argv)
{
    // setsockopt( socket, SOL_SOCKET, SO_SNDTIMEO, ( char * )&nNetTimeout, sizeof( int ) );
    // setsockopt( socket, SOL_SOCKET, SO_RCVTIMEO, ( char * )&nNetTimeout, sizeof( int ) );
    return poll_server(argv);
    return select_server(argv);
    return normal_server(argv);
}
