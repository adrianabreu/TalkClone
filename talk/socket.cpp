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

    server_ = true;
    //We have to listen for a 5 connections
    int result = listen(fd_, 5);

    if (result < 0)
        throw std::system_error(errno, std::system_category(),
                                "no se pudo escuchar");

}

Socket::Socket(const sockaddr_in& address, const sockaddr_in& remote)
{
    normalSocket(address);
    server_ = false;
    int result = connect(fd_,reinterpret_cast<const sockaddr*>(&remote),
                         sizeof(remote));

    if (result < 0)
        throw std::system_error(errno, std::system_category(),
                                "no se pudo conectar");
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

Socket::~Socket()
{
    //Finalizamos el descriptor del fichero para terminar la conexion
    close(fd_);
}

void Socket::handleConnections()
{
    fd_ = accept(fd_,nullptr,nullptr);
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

void Socket::sendTo(const Message& message, const sockaddr_in& address)
{
    int result = write(fd_, static_cast<const void*>(&message), sizeof(message));

    if ( result < 0 )
        throw std::system_error(errno, std::system_category(), "falló write: ");
}

void Socket::receiveFrom(Message& message, sockaddr_in& address)
{
    socklen_t src_len = sizeof(address);

    int result = read(fd_,static_cast<void*>(&message), sizeof(message));

    if (result < 0)
        throw std::system_error(errno, std::system_category(), "falló read: ");

    if (result == 0)
        throw std::system_error(errno, std::system_category(), "Connection was over: ");
}

Socket& Socket::operator=(Socket&& older)
{
    fd_=older.getFd();
    older.setFd(-1);
    server_ = older.actingLikeServer();

    return *this;
}
