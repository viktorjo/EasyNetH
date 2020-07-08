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
    
}

AbstractSocket::~AbstractSocket()
{
    
}

bool AbstractSocket::isAddrOk(std::string addr)
{
	return inet_addr(addr.data()) != -1;
}

bool AbstractSocket::bind(u_int16_t port)
{
    
    switch (this->socket_type) {
        case TcpSocket:
            return bindTcp(port);
            break;
        case UdpSocket:
            //return bindUdp(port);
            break;
        default:
            socket_error = SOCKET_TYPE_NOT_SUPPORTED;
			break;
    }
    return false;
}

bool AbstractSocket::listen(int maxIncomingConnection)
{
    if (socket_type != TcpSocket) {
        socket_error = TCP_SOCKET;
        return false;
    }
    if (socket_state != BOUND) {
        socket_error = SOCKET_NOT_BOUND;
        return false;
    }
    
    if (::listen(socket_internal.socket_fd, maxIncomingConnection) < 0)
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
    socket_out.socket_fd = ::accept(socket_internal.socket_fd, (struct sockaddr *) &socket_out.address, &socket_out.addrlen);
    if (socket_out.socket_fd < 0) {
        socket_error = ACCEPT_FAILED;
        return false;
    }
    socket_error = NO_ERROR;
    socket_state = CONNECTED;
    return true;
}

bool AbstractSocket::connectToHost(const std::string &hostName, u_int16_t port)
{
	if (!isAddrOk(hostName)) {
		socket_error = BAD_ADDRESS;
		perror("Bad address.");
		return false;
	}
	
	socket_out.socket_fd = socket(AF_INET,SOCK_STREAM,0);
	if (socket_out.socket_fd == 0)
    {
        socket_error = SOCKET_CREATION_FAILED;
        perror("Could not create socket");
        return false;
    }
	socket_out.address.sin_addr.s_addr = inet_addr(hostName.data());
	socket_out.address.sin_port = htons( port );
	
	if (connect(socket_internal.socket_fd, (struct sockaddr *) &socket_internal.address, sizeof(socket_internal.address)) != 0)
	{
		socket_error = CONNECT_FAILED;
		perror("Failed to connect to server");
		return false;
	}
	
    return true;
}

void AbstractSocket::closeSockets()
{
	close(socket_internal.socket_fd);
	close(socket_out.socket_fd);
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
    ssize_t bytes_read = ::recv(socket_out.socket_fd, data, maxSize, 0);
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
    
    ssize_t bytes_sent = ::send(socket_out.socket_fd, data, size, 0);
    if (bytes_sent < 0)
    {
        socket_error = SEND_FAILED;
        return -1;
    }
    socket_error = NO_ERROR;
    return bytes_sent;
}


/**
 *
 * Private Functions
 *
 **/

bool AbstractSocket::bindTcp(uint16_t port)
{
    socket_internal.socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if (socket_internal.socket_fd == 0)
    {
        socket_error = SOCKET_CREATION_FAILED;
        perror("Could not create socket");
        return false;
    }
    else
    {
        socket_internal.address.sin_port = htons( port );
        if (::setsockopt(socket_internal.socket_fd, SOL_SOCKET, SO_REUSEADDR, &socket_internal.opt, sizeof(socket_internal.opt)))
        {
            perror("setsockopt");
            return false;
        }
    }
    
    if(::bind(socket_internal.socket_fd, (struct sockaddr *) &socket_internal.address, sizeof(socket_internal.address)) < 0)
    {
        perror("Could not bind");
        socket_error = COULD_NOT_BIND;
        return false;
    }
    socket_error = NO_ERROR;
    socket_state = BOUND;
    return true;
}



