#include "socket.h"

#define LOCALPORT 5500
#define REMOTEPORT 6000



sockaddr_in make_ip_address(const std::string& ip_address, int port){
    sockaddr_in tmp;

    tmp.sin_family = AF_INET;
    const char * c = ip_address.c_str();
    inet_aton(c,&tmp.sin_addr);
    tmp.sin_port = htons(port);

    return tmp;
}

int main(void){

    //Preparar estructura local
    sockaddr_in sin_local{};    // Porque se recomienda inicializar a 0
    sin_local = make_ip_address("0.0.0.0",LOCALPORT);

    Socket* socket;

    try {

        socket = new Socket(sin_local);

    } catch (std::system_error& e) {

        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;

        return 3;    // Error. Termina el programa siempre con un valor > 0
    }



    //Preparar socket remoto
    sockaddr_in sin_remote{}; // Porque se recomienda inicializar a 0
    sin_remote = make_ip_address("0.0.0.0",REMOTEPORT);

    //inet_aton("1", &remote_address.sin_addr);

    //Lectura
    Message message;
    std::string message_text("");

    while(1){

        std::getline(std::cin,message_text);

        if ( message_text == "/quit" || std::cin.eof() )
            break;

        memset (message.text,0,sizeof(message.text));
        message_text.copy(message.text, sizeof(message.text) - 1, 0);

        try {

            socket->send_to(message, sin_remote);

        } catch (std::system_error& e) {

            std::cerr << program_invocation_name << ": " << e.what()
            << std::endl;

            return 6;    // Error. Termina el programa siempre con un valor > 0

        }

        try {

            socket->receive_from(message, sin_remote);

        } catch (std::system_error& e) {

            std::cerr << program_invocation_name << ": " << e.what()
            << std::endl;
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
