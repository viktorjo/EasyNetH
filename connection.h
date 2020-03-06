//
//  connection.h
//  Network Handler
//
//  Created by Viktor Johansson on 2019-09-17.
//

#ifndef connection_h
#define connection_h

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Connection {
public:
    Connection();
    ~Connection();
    /*	
    enum ConnectionType {
        INCOMING = 0,
        OUTGOING
    };
     */
    int createTcpServer(const int &port);
    
private:
    //int mConnectionType = INCOMING;
    int mSocketFiledescriptor = 0;
};

#endif /* connection_h */
