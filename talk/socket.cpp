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
     * We have to start the file descriptor to a non
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
        std::cout << "No se pudo conectar, entering server mode" << std::endl;
        serverMode(&address);
    }
}

void Socket::normalSocket(const sockaddr_in& address)
{
    // Crear el socket local
    fd_ = socket(AF_INET, SOCK_STREAM, 0);

    if ( fd_ < 0 )
        throw std::system_error(errno, std::system_category(),
                                "no se pudo crear el socket");

    //Asignar la dirección con bind
    int result = bind(fd_, reinterpret_cast<const sockaddr*>(&address),
                      sizeof(address));

    if ( result < 0 )
        throw std::system_error(errno, std::system_category(),"falló bind: ");


}

void Socket::serverMode(const sockaddr_in *address)
{
    server_ = true;
    //We have to listen for a 5 connections
    int result = listen(fd_, 5);

    if (result < 0)
        throw std::system_error(errno, std::system_category(),
                                "no se pudo escuchar");
    else
        std::cout << "Entering server mode... Listening on " <<
                      ntohs(address->sin_port) << std::endl;

}

Socket::~Socket()
{
    //Finalizamos el descriptor del fichero para terminar la conexion
    close(fd_);
}

void Socket::handleConnections(sockaddr_in *remote)
{
    //Accept returns a new socket with the connection
    socklen_t ssize = sizeof(*remote);
    fd_ = accept(fd_,reinterpret_cast<sockaddr*>(remote),&ssize);
    std::cout << "Remote user: " << htons(remote->sin_port) << " has connected" << std::endl;
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

    if ( result < 0 )
        std::cout << "Your partner has diconnected" << std::endl;
        //throw std::system_error(errno, std::system_category(), "falló write: ");
}

void Socket::receiveFrom(Message& message)
{
    int result = read(fd_,static_cast<void*>(&message), sizeof(message));

    if (result < 0)
        throw std::system_error(errno, std::system_category(), "falló read: ");

    if (result == 0) //If read = 0, socket was closed
        throw std::system_error(errno, std::system_category(), "Connection was over: ");
}

Socket& Socket::operator=(Socket&& older)
{
    fd_=older.getFd();
    older.setFd(-1);
    server_ = older.actingLikeServer();

    return *this;
}
