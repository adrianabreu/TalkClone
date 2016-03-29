#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "socket.h"

class TCPServer : public Socket
{
public:
    TCPServer();
    TCPServer(const sockaddr_in& address);
    void serverMode(const sockaddr_in *address);
    void handleConnections(sockaddr_in *remote);
};

#endif // TCPSERVER_H
