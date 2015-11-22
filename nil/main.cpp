//
//  main.cpp
//  nil
//
//  Created by nil on 11/21/15.
//  Copyright © 2015 nil. All rights reserved.
//

#include <pthread.h>
#include <unistd.h>

#include <iostream>

extern void* server(void* argv);
extern void* client(void* argv);

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    pthread_t server_id, client_id;
    pthread_create(&server_id, nullptr, server, nullptr);
    pthread_create(&client_id, nullptr, client, nullptr);
    
    void* result;
    pthread_join(server_id, &result);
    
    for ( ; ; )
    {
        sleep(8);
        break;
    }
    
    std::cout << "main over" << std::endl;
    return 0;
}
