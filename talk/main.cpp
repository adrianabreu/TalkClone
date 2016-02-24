#include "socket.h"

#define LOCALPORT 5500
#define REMOTEPORT 6000



sockaddr_in make_ip_address(const std::string& ip_address, int port){
    sockaddr_in tmp{};

    tmp.sin_family = AF_INET;
    const char * c = ip_address.c_str();
    inet_aton(c,&tmp.sin_addr);
    tmp.sin_port = htons(port);

    return tmp;
}

int main(void){

    //Preparar estructura local
    sockaddr_in sin_local;
    sin_local = make_ip_address("0.0.0.0",LOCALPORT);

    Socket* socket;

    try {

        socket = new Socket(sin_local);

    } catch (std::bad_alloc& ba){

        std::cerr << program_invocation_name << ": error creating socket " << ba.what()
        << std::endl;

        return 3;

    }catch (std::system_error& e) {

        if (errno == ENOBUFS || errno == ENOMEM) {

            std::cerr << program_invocation_name << ": Not enough memory for creating Socket "
            << std::endl;

        } else {

            std::cerr << program_invocation_name << ": " << e.what()
            << std::endl;

        }


        return 3;    // Error. Termina el programa siempre con un valor > 0
    }


    //Preparar socket remoto
    sockaddr_in sin_remote;
    sin_remote = make_ip_address("0.0.0.0",REMOTEPORT);

    //Estructura de mensaje
    Message message;
    std::string message_text("");

    std::getline(std::cin,message_text);

    while(message_text != "/quit" && !std::cin.eof()){

        //Limpiamos el mensaje previo para evitar caracteres extraños
        message_text.copy(message.text, sizeof(message.text) - 1, 0);
        message.text[message_text.length()] = '\0';

        try {

            socket->sendTo(message, sin_remote);

        } catch (std::system_error& e) {

            std::cerr << program_invocation_name << ": " << e.what()
                      << std::endl;
        }

        try {

            socket->receiveFrom(message, sin_remote);

        } catch (std::system_error& e) {

            std::cerr << program_invocation_name << ": " << e.what()
            << std::endl;
        }

        // Mostrar el mensaje recibido en la terminal
        char* remote_ip = inet_ntoa(sin_remote.sin_addr);
        int remote_port = ntohs(sin_remote.sin_port);

        /* Para aumentar la seguridad de la aplicacion,
         * añadimos un fin de cadena que limite la lectura
         * al tamaño del buffer, por si el mensaje no está
         * delimitado de por sí
         */
        message.text[1023] = '\0';

        std::cout << "El sistema " << remote_ip << ":" << remote_port <<
        " envió el mensaje '" << message.text << "'" << std::endl;

        std::getline(std::cin, message_text);

    }
    return 0;
}
