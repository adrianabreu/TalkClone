#include "tcpserver.h"

TCPServer::TCPServer() {}

TCPServer::TCPServer(const TCPServer& older) : Socket()
{ //If you do not call socket, a warning is launched
    fd_ = older.getFd();
}

TCPServer::~TCPServer()
{
    //All the threads we have created must be destroyed
    for(unsigned int i = 0; i < threads_.size(); ++i)
        requestCancellation(threads_[i]);
}

TCPServer::TCPServer(const std::string& ip_address, int port)
{
    sockaddr_in address = makeIpAddress(ip_address, port);
    normalSocket(address);
    int result = listen(fd_, 5);

    if (result < 0)
        throw std::system_error(errno, std::system_category(),
                                "listen error");
    else
        std::cout << "Entering server mode... Listening on " <<
                      ntohs(address.sin_port) << std::endl;
}

int TCPServer::handleConnections(sockaddr_in *remote)
{
    int aux = -1; // no valid fd
    //Accept returns a new socket with the connection
    socklen_t ssize = sizeof(*remote);
    int tempfd = accept(fd_,reinterpret_cast<sockaddr*>(remote),&ssize);

    if (tempfd < 0) {
        throw std::system_error(errno, std::system_category(),
                                " Error on accept");
    } else {
        //std::cout << "Remote user: " << inet_ntoa(remote->sin_addr)
        //        << " has connected from port " << ntohs(remote->sin_port)
        //          << std::endl;
        aux = tempfd;
    }
    return aux; //We return the new socket
}

TCPServer& TCPServer::operator =(TCPServer&& older)
{
    fd_= older.getFd();
    older.setFd(-1);

    return *this;
}
