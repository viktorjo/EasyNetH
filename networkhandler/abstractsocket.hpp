//
//  abstractsocket.hpp
//  networkhandler
//
//  Created by Viktor Johansson on 2020-03-07.
//

#ifndef abstractsocket_hpp
#define abstractsocket_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class AbstractSocket {
public:
    
    typedef int8_t BYTE;
    enum SocketType {
        UdpSocket,
        TcpSocket
    };
    
    enum SocketError
    {
        NO_ERROR,
        SOCKET_TYPE_NOT_SUPPORTED,
        SOCKET_CREATION_FAILED,
        SOCKET_ALREADY_IN_USE,
        COULD_NOT_BIND,
        TCP_SOCKET,
        UDP_SOCKET,
        SOCKET_NOT_BOUND,
        LISTEN_FAILED,
        ACCEPT_FAILED,
        NOT_CONNECTED,
        READ_FAILED,
        SEND_FAILED,
		BAD_ADDRESS,
		CONNECT_FAILED
    };
    
    AbstractSocket(AbstractSocket::SocketType socketType);
    virtual ~AbstractSocket();
    
	
	static bool isAddrOk(std::string addr);
    //bool bind(const std::string &hostName, u_int16_t port);
    bool bind(u_int16_t port);
    
    bool listen(int maxIncomingConnections);
    bool accept();
    	
	
	bool connectToHost(const std::string &hostName, u_int16_t port);
	
	void closeSockets();
    
    enum SocketError error() const { return socket_error;}
    
    //ssize_t readAll();
    ssize_t readData(BYTE *data, size_t maxSize);
    ssize_t sendData(const BYTE *data, size_t size);
    
private:
    enum SocketState
    {
        NOT_IN_USE = 0,
        BOUND,
        LISTENING,
        CONNECTED
    };
    
    struct socket_data
    {
        int socket_fd = 0;
        struct sockaddr_in address;
        socklen_t addrlen = sizeof(address);
        int opt = 1;
    };
    /*
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt;
    */
    
    enum SocketType socket_type;
    enum SocketState socket_state = NOT_IN_USE;
    enum SocketError socket_error = NO_ERROR;
    
    struct socket_data socket_internal; // Handle connection and incoming data.
    struct socket_data socket_out;
    
    bool bindTcp(u_int16_t port);
    //bool bindUdp(u_int16_t port  );
    
    
};

#endif /* abstractsocket_hpp */
