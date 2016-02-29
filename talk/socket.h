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
    int fd_;

public:
    Socket();
    Socket(const sockaddr_in& address);
    Socket(Socket &older);
    ~Socket();
    int getFd();
    void setFd(int newFd);
    void sendTo(const Message& message, const sockaddr_in& address);
    void receiveFrom(Message& message, sockaddr_in& address);
    Socket& operator =(Socket&& older);
};

#endif // SOCKET_H
