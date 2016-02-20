#include "socket.h"

Socket::Socket(const sockaddr_in& address)
{
    // Crear el socket local
    fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd_ < 0) {
        std::cerr << "no se pudo crear el socket: " <<
        std::strerror(errno) << std::endl;
        //return 3;    // Error. Termina el programa siempre con un valor > 0
    }
    //Asignar la dirección con bind
    int result = bind(fd_, reinterpret_cast<const sockaddr*>(&address),
        sizeof(address));
    if ( result < 0 ) {
        std::cerr << "falló bind: " << std::strerror(errno) << std::endl;
        //return 5;    // Error. Termina el programa siempre con un valor > 0
    }
}

Socket::~Socket()
{
    close(fd_);
}

void Socket::send_to(const Message& message, const sockaddr_in& address)
{
    int result = sendto(fd_, reinterpret_cast<const void*>(&message), sizeof(message),
                        0,reinterpret_cast<const sockaddr*>(&address),sizeof(address));

    if ( result < 0 ) {
        std::cerr << "falló sendto: " << std::strerror(errno) << std::endl;
        //return 6;
    }
}

void Socket::receive_from(Message& message, sockaddr_in& address)
{
    socklen_t src_len = sizeof(address);
    int result = recvfrom(fd_,reinterpret_cast<void*>(&message), sizeof(message), 0,
        reinterpret_cast<sockaddr*>(&address), &src_len);
    if (result < 0) {
        std::cerr << "falló recvfrom: " << std::strerror(errno) << std::endl;
        //return 8;
    }
}
