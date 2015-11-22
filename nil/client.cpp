//
//  client.cpp
//  nil
//
//  Created by nil on 11/21/15.
//  Copyright © 2015 nil. All rights reserved.
//

#include "client.hpp"

#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <iostream>

void* client(void* argv)
{
    sleep(8);
    std::cout << "client main" << std::endl;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    //    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8080);
    
    if (int temp = connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        printf("connect error: %d \n", temp);
    }
    
    char text[] = "<this is test>";
    ssize_t count = send(fd, text, sizeof(text), 0);
    printf("send count %zd, %s \n", count, text);
    
    char buff[1024] = { 0 };
    count = recv(fd, buff, sizeof(buff), 0);
    printf("recv count %zd, %s \n", count, buff);
    
    count = send(fd, text, sizeof(text), 0);
    printf("send count %zd, %s \n", count, text);
    
    sleep(8);
    close(fd);
    std::cout << "client over" << std::endl;
    pthread_exit(nullptr);
    return nullptr;
}