#include "socket.h"

Socket::Socket(const sockaddr_in& address)
{
    // Crear el socket local
    fd_ = socket(AF_INET, SOCK_DGRAM, 0);

    if ( fd_ < 0 )
        throw std::system_error(1, std::system_category(),
                                "no se pudo crear el socket");

    //Asignar la dirección con bind
    int result = bind(fd_, reinterpret_cast<const sockaddr*>(&address),
                      sizeof(address));

    if ( result < 0 )
        throw std::system_error(2, std::system_category(),"falló bind: ");
}

Socket::~Socket()
{
    //Finalizamos el descriptor del fichero para terminar la conexion
    close(fd_);
}

void Socket::sendTo(const Message& message, const sockaddr_in& address)
{
    int result = sendto(fd_, static_cast<const void*>(&message), sizeof(message),
                        0,reinterpret_cast<const sockaddr*>(&address),sizeof(address));

    //SIGPIPE error can be avoided ignoring this
    //Normally sigpipe is ignored
    if ( result < 0 )
        throw std::system_error(errno, std::system_category(), "falló sendto: ");
}

void Socket::receiveFrom(Message& message, sockaddr_in& address)
{
    socklen_t src_len = sizeof(address);

    int result = recvfrom(fd_,static_cast<void*>(&message), sizeof(message), 0,
        reinterpret_cast<sockaddr*>(&address), &src_len);

    if (result < 0)
        throw std::system_error(errno, std::system_category(), "falló recvfrom: ");
}
