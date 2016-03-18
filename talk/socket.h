#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <system_error>

// Messages structure
struct Message {

    char text[1024];

};

//Helper function for building sockaddr_in struct
sockaddr_in makeIpAddress(const std::string& ip_address, int port);

class Socket
{

private:
    bool server_; //This indicates if we are acting like a TCP server
    int fd_; //Socket
    void normalSocket(const sockaddr_in& address);

public:
    Socket();
    Socket(const sockaddr_in& address);
    //This tries to connect and if it isn't possible, starts to listen
    Socket(const sockaddr_in& address, const sockaddr_in& remote);
    Socket(const Socket&);
    ~Socket();

    int getFd() const;
    void setFd(int newFd);

    bool actingLikeServer() const;
    void serverMode(const sockaddr_in *address);
    void handleConnections(sockaddr_in *remote);

    void sendTo(const Message& message);
    void receiveFrom(Message& message);

    Socket& operator=(Socket&& older);
    Socket& operator=(Socket& older);
};

#endif // SOCKET_H
