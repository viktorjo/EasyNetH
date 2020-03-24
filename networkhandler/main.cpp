//
//  main.cpp
//  networkhandler
//
//  Created by Viktor Johansson on 2019-09-17.
//

#include <iostream>
//#include "connection.h"
//#include "tcpsocket.h"
#include "abstractsocket.hpp"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

void abstr_call()
{
    AbstractSocket::BYTE data[1024] = { 0 };
    ssize_t bytes_read = 0;
    ssize_t bytes_sent = 0;
    AbstractSocket test(AbstractSocket::TcpSocket);
    
    bool success = false;
    success = test.bind(PORT);
    if (success) {
        std::cout << "Bound socket\n";
    }
    else {
        std::cout << "Could not bind socket\n";
    }
        
    success = test.listen(1);
    if (success) {
        std::cout << "Bound listening to socket\n";
    }
    else {
        std::cout << "Could not listen to socket\n" << test.error();
    }
    success = test.accept();
    if (success) {
        std::cout << "Socket Accepted\n";
    }
    else {
        std::cout << "Could not accept socket.\n";
    }
    bytes_read = test.readData(data, 1024);
    if (bytes_read > 0)
    {
        std::cout << "Read " << bytes_read << " bytes of data\n";
        std::cout << data;
    }
    else {
        std::cout << "Could not read data\n";
    }
    bytes_sent = test.sendData(data, bytes_read);
    if (bytes_sent > 0)
    {
        std::cout << "Sent " << bytes_read << " bytes of data\n";
        std::cout << data;
    }
    else {
        std::cout << "Could not send socket\n";
    }
}

void nrml_call()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    
    
}


int main(int argc, const char * argv[]) {
    
    abstr_call();
    //nrml_call();
    
    return 0;
}


