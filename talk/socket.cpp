#include "socket.h"

sockaddr_in makeIpAddress(const std::string& ip_address, int port){
    sockaddr_in tmp{};

    tmp.sin_family = AF_INET;
    const char * c = ip_address.c_str();
    inet_aton(c,&tmp.sin_addr);
    tmp.sin_port = htons(port);

    return tmp;
}

Socket::Socket()
{
    /*
     * We have to initialize the file descriptor to a non
     * valid value
     */
    fd_ = -1;
    server_ = false;
}

Socket::Socket(const sockaddr_in& address)
{
    normalSocket(address);

    serverMode(&address);

}

Socket::Socket(const sockaddr_in& address, const sockaddr_in& remote)
{
    normalSocket(address);
    server_ = false;
    int result = connect(fd_,reinterpret_cast<const sockaddr*>(&remote),
                         sizeof(remote));

    if (result < 0) {
        std::cout << "Couldn't connect, entering server mode" << std::endl;
        serverMode(&address);
    } else {
        std::cout << "Connected to " << inet_ntoa(remote.sin_addr) << std::endl;
    }
}

void Socket::normalSocket(const sockaddr_in& address)
{
    // Create local socket
    fd_ = socket(AF_INET, SOCK_STREAM, 0);

    if ( fd_ < 0 )
        throw std::system_error(errno, std::system_category(),
                                "it wasn't possible to create the socket");

    //Bind the socket to a known direction
    int result = bind(fd_, reinterpret_cast<const sockaddr*>(&address),
                      sizeof(address));

    if ( result < 0 )
        throw std::system_error(errno, std::system_category(),"bind error: ");


}

void Socket::serverMode(const sockaddr_in *address)
{
    server_ = true;
    //Actually we just need to listen for 1 connection
    int result = listen(fd_, 1);

    if (result < 0)
        throw std::system_error(errno, std::system_category(),
                                "listen error");
    else
        std::cout << "Entering server mode... Listening on " <<
                      ntohs(address->sin_port) << std::endl;

}

Socket::~Socket()
{
    //We just need to close the file descriptor for end connection
    close(fd_);
}

void Socket::handleConnections(sockaddr_in *remote)
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

int Socket::getFd()
{
    return fd_;
}

void Socket::setFd(int newFd)
{
    fd_ = newFd;
}

bool Socket::actingLikeServer()
{
    return server_;
}

void Socket::sendTo(const Message& message)
{
    int result = write(fd_, static_cast<const void*>(&message), sizeof(message));

    if ( result < 0 ) {
        std::cout << "Your partner has diconnected" << std::endl;
        throw std::system_error(errno, std::system_category(), "write error: ");
    }
}

void Socket::receiveFrom(Message& message)
{
    //Deal with incomplete messages
    int result = read(fd_,static_cast<void*>(&message), sizeof(message));

    if (result < 0)
        throw std::system_error(errno, std::system_category(), "read error: ");

    if (result == 0) //If read = 0, socket was closed
        throw std::system_error(errno, std::system_category(),
                                "Connection was over: ");
}

Socket& Socket::operator=(Socket&& older)
{
    fd_=older.getFd();
    older.setFd(-1);
    server_ = older.actingLikeServer();

    return *this;
}
