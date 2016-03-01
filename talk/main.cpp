#include "socket.h"

#define LOCALPORT 5500
#define REMOTEPORT 5500

#define SUCCESS 0
#define ERR_SOCKET 3

void getandSendMessage(Socket *local, Message message, sockaddr_in remote,
                       std::string *message_text, bool *endOfLoop)
{
    //Limpiamos el mensaje previo para evitar caracteres extraños
    std::getline(std::cin,*message_text);

    if(*message_text == "/quit" || std::cin.eof())
        *endOfLoop = true;

    if (!*endOfLoop) {
        message_text->copy(message.text, sizeof(message.text) - 1, 0);
        message.text[message_text->length()] = '\0';
        local->sendTo(reinterpret_cast<const Message&>(message),
                 reinterpret_cast<const sockaddr_in&>(remote));
    }

}

void receiveAndShowMessage(Socket *socket, Message *message, sockaddr_in sin_remote)
{

    socket->receiveFrom(*message, sin_remote);

    // Mostrar el mensaje recibido en la terminal
    char* remote_ip = inet_ntoa(sin_remote.sin_addr);
    int remote_port = ntohs(sin_remote.sin_port);

    /* Para aumentar la seguridad de la aplicacion,
     * añadimos un fin de cadena que limite la lectura
     * al tamaño del buffer, por si el mensaje no está
     * delimitado de por sí
     */
    message->text[1023] = '\0';

    std::cout << "El sistema " << remote_ip << ":" << remote_port <<
    " envió el mensaje '" << message->text << "'" << std::endl;
}

int main(void){

    int aux = SUCCESS;
    //Preparar estructura local
    sockaddr_in sin_local;
    sin_local = makeIpAddress("0.0.0.0",LOCALPORT);

    Socket local;

    try {
         local = Socket(sin_local);

    }catch (std::system_error& e) {

        if (errno == ENOBUFS || errno == ENOMEM) {
            std::cerr << program_invocation_name << ": Not enough memory for creating Socket "
            << std::endl;
        } else {
            std::cerr << program_invocation_name << ": " << e.what()
            << std::endl;
        }

        throw;    // Error. Termina el programa siempre con un valor > 0
    }

    //Preparar socket remoto
    sockaddr_in sin_remote;
    sin_remote = makeIpAddress("0.0.0.0",REMOTEPORT);

    //Estructura de mensaje
    Message message;
    std::string message_text("");

    bool endOfLoop = false;

    while(!endOfLoop){

        try {
            getandSendMessage(&local,message,sin_remote,&message_text, &endOfLoop);
            if(!endOfLoop)
                receiveAndShowMessage(&local,&message,sin_remote);
        } catch (std::system_error& e) {
            std::cerr << program_invocation_name << ": " << e.what()
            << std::endl;
        }

    }

    return aux;
}
