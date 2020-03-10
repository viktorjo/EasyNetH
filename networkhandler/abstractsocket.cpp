//
//  abstractsocket.cpp
//  networkhandler
//
//  Created by Viktor Johansson on 2020-03-07.
//

#include "abstractsocket.hpp"

#define RECV_BUFFER_SIZE 1024

AbstractSocket::AbstractSocket(AbstractSocket::SocketType socketType)
{
    this->socket_type = socketType;
    socket_internal.address.sin_family = AF_INET;
    socket_internal.address.sin_addr.s_addr = INADDR_ANY;
    //socket_out = socket_internal;
}

AbstractSocket::~AbstractSocket()
{
    
}

bool AbstractSocket::bind(u_int16_t port)
{
    
    address.sin_port = htons( port );
    
    server_fd = socket(AF_INET,
                        SOCK_STREAM,
                        0);
    if (server_fd == 0)
    {
        socket_error = SOCKET_CREATION_FAILED;
        perror("Could not create socket");
        return false;
    }
    else
    {
        if (::setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        {
            perror("setsockopt");
            return false;
        }
    }
    
    if(::bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        perror("Could not bind");
        socket_error = COULD_NOT_BIND;
        return false;
    }
    socket_error = NO_ERROR;
    socket_state = BINDED;
    return true;
}

bool AbstractSocket::listen(int maxIncomingConnection)
{
    if (socket_type != TcpSocket) {
        socket_error = TCP_SOCKET;
        return false;
    }
    if (socket_state != BINDED) {
        socket_error = SOCKET_NOT_BOUND;
        return false;
    }
    int success = ::listen(socket_internal.socket_fd, maxIncomingConnection) < 0;
    
    if (!success)
    {
        socket_error = LISTEN_FAILED;
        return false;
    }
    socket_error = NO_ERROR;
    socket_state = LISTENING;
    return true;
}

bool AbstractSocket::accept()
{
    if (socket_type != TcpSocket) {
        socket_error = TCP_SOCKET;
        return false;
    }
    //socket_out.socket_fd = ::accept(socket_internal.socket_fd, (struct sockaddr *) &socket_out.address, &socket_out.addrlen);
    //if (socket_out.socket_fd < 0) {
        socket_error = ACCEPT_FAILED;
        return false;
    //}
    socket_error = NO_ERROR;
    socket_state = CONNECTED;
    return true;
}

void AbstractSocket::connectToHost(const std::string &hostName, u_int16_t port)
{
    
}
/*
ssize_t AbstractSocket::readAll()
{
    static BYTE recv_buffer[RECV_BUFFER_SIZE];
    ssize_t bytes_read = ::recv(this->socket_internal.socket_fd, recv_buffer, (size_t) RECV_BUFFER_SIZE, 0);
    if (bytes_read < 0) {
        this->socket_error = READ_FAILED;
        return -1;
    }
    if (this->socket_read_buffer == nullptr)
    {
        this->socket_error = BUFFER_NULL_PTR;
        return -1;
    }
    
    for (int index = 0; index < bytes_read ; index++) {
        this->socket_read_buffer->push_back(recv_buffer[index]);
    }
    return this->socket_read_buffer->size();
}
*/

ssize_t AbstractSocket::readData(BYTE *data, size_t maxSize)
{
    if (socket_state != CONNECTED) {
        socket_error = NOT_CONNECTED;
        return -1;
    }
    ssize_t bytes_read = ::recv(socket_internal.socket_fd, data, maxSize, 0);
    if (bytes_read < 0)
    {
        socket_error = READ_FAILED;
        return -1;
    }
    socket_error = NO_ERROR;
    return bytes_read;
}

ssize_t AbstractSocket::sendData(const BYTE *data, size_t size)
{
    if (socket_state != CONNECTED) {
        socket_error = NOT_CONNECTED;
        return -1;
    }
    
    //ssize_t bytes_sent = ::send(socket_out.socket_fd, data, size, 0);
    //if (bytes_sent < 0)
    //{
        socket_error = SEND_FAILED;
        return -1;
    //}
    socket_error = NO_ERROR;
    //return bytes_sent;
    return 0;
}


/**
 *
 * Private Functions
 *
 **/

/*
AbstractSocket::AbstractSocket(const AbstractSocket &other, enum SocketState state)
{
    this->socket_internal = other.socket_internal;
    this->socket_out = other.socket_out;
    this->socket_type = other.socket_type;
    this->socket_state = state;
}
*/



