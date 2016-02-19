#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>

#include <cerrno>
#include <cstring>

#define LOCALPORT 5500
#define REMOTEPORT 6000

// Estructura de los mensajes
struct Message {

    //...                    // Otros campos del mensaje

    char text[1024];

    //...                    // Otros campos del mensaje

};

int main(void){

    // Crear el socket local
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        std::cerr << "no se pudo crear el socket: " <<
        std::strerror(errno) << std::endl;
        return 3;    // Error. Termina el programa siempre con un valor > 0
    }

    //Preparar estructura local
    sockaddr_in sin_local{};    // Porque se recomienda inicializar a 0
    sin_local.sin_family = AF_INET;
    sin_local.sin_addr.s_addr = htonl(INADDR_ANY);
    sin_local.sin_port = htons(LOCALPORT);

    //Asignar la dirección con bind
    int result = bind(fd, reinterpret_cast<const sockaddr*>(&sin_local),
        sizeof(sin_local));
    if (result < 0) {
        std::cerr << "falló bind: " << std::strerror(errno) << std::endl;
        return 5;    // Error. Termina el programa siempre con un valor > 0
    }

    //Preparar socket remoto
    sockaddr_in sin_remote{}; // Porque se recomienda inicializar a 0
    sin_remote.sin_family = AF_INET;
    sin_remote.sin_port = htons(REMOTEPORT);
    sin_remote.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    socklen_t src_len = sizeof(sin_remote);
    //inet_aton("1", &remote_address.sin_addr);

    //Lectura
    // Enviar un mensaje "¡Hola, mundo!" al socket remoto
    Message message;
    std::string message_text("¡Hola, mundo!");
    while(1){

        std::getline(std::cin,message_text);
        if(message_text == "/quit" )
            break;


        memset (message.text,0,sizeof(message.text));
        message_text.copy(message.text, sizeof(message.text) - 1, 0);


        int result = sendto(fd, &message, sizeof(message), 0,reinterpret_cast<const sockaddr*>(&sin_remote),sizeof(sin_remote));
        if (result < 0) {
            std::cerr << "falló sendto: " << std::strerror(errno) << std::endl;
            return 6;
        }
        result = recvfrom(fd, &message, sizeof(message), 0,
            reinterpret_cast<sockaddr*>(&sin_remote), &src_len);
        if (result < 0) {
            std::cerr << "falló recvfrom: " << std::strerror(errno) << std::endl;
            return 8;
        }

        // Mostrar el mensaje recibido en la terminal
        char* remote_ip = inet_ntoa(sin_remote.sin_addr);
        int remote_port = ntohs(sin_remote.sin_port);
        message.text[254] = '\0';
        std::cout << "El sistema " << remote_ip << ":" << remote_port <<
        " envió el mensaje '" << message.text << "'" << std::endl;
    }
    return 0;
}
