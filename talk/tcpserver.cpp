#include "tcpserver.h"

TCPServer::TCPServer()
{
    fd_ = -1;
}
TCPServer::TCPServer(const std::string& ip_address, int port)
{
    sockaddr_in& address = makeIpAddress(ip_address, port);
    normalSocket(address);
    int result = listen(fd_, 5);

    if (result < 0)
        throw std::system_error(errno, std::system_category(),
                                "listen error");
    else
        std::cout << "Entering server mode... Listening on " <<
                      ntohs(address.sin_port) << std::endl;
}

TCPServer::TCPServer(const std::string& ip_address, int port)
{
    sockaddr_in& address = makeIpAddress(ip_address, port);
    normalSocket(address);
    int result = listen(fd_, 5);

    if (result < 0)
        throw std::system_error(errno, std::system_category(),
                                "listen error");
    else
        std::cout << "Entering server mode... Listening on " <<
                      ntohs(address.sin_port) << std::endl;
}

void TCPServer::handleConnections(sockaddr_in *remote)
{
    //Accept returns a new socket with the connection
    socklen_t ssize = sizeof(*remote);
    fd_ = accept(fd_,reinterpret_cast<sockaddr*>(remote),&ssize);

    if (fd_ < 0) {
        throw std::system_error(errno, std::system_category(),
                                " Error on accept");
    } else {
        std::cout << "Remote user: " << inet_ntoa(remote->sin_addr)
                  << " has connected from port " << ntohs(remote->sin_port)
                  << std::endl;
    }
}
