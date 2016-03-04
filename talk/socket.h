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

// Estructura de los mensajes
struct Message {

    char text[1024];

};

//Helper function
sockaddr_in makeIpAddress(const std::string& ip_address, int port);

class Socket
{
private:
    bool server_; //This indicates if we are acting like a TCP server
    int fd_;
    void normalSocket(const sockaddr_in& address);

public:
    Socket();
    Socket(const sockaddr_in& address);
    Socket(const sockaddr_in& address, const sockaddr_in& remote);
    ~Socket();
    int getFd();
    void handleConnections();
    void setFd(int newFd);
    bool actingLikeServer();
    void sendTo(const Message& message, const sockaddr_in& address);
    void receiveFrom(Message& message, sockaddr_in& address);
    Socket& operator=(Socket&& older);
};

#endif // SOCKET_H
