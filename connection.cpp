//
//  connection.cpp
//  Network Handler
//
//  Created by Viktor Johansson on 2019-09-17.
//
#include "connection.h"
#include <stdio.h>

Connection::Connection() {
    printf("Connection created.\n");
}

Connection::~Connection() {
    
}

int Connection::createTcpServer(const int &port) {
    int sockfd; // Socket file descriptor
    struct sockaddr_in addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        return sockfd;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    return 0;
}
