#include "socket.h"
#include <thread>
#include <pthread.h>

#define LOCALPORT 6000
#define REMOTEPORT 5500

#define SUCCESS 0
#define ERR_SOCKET 3

/*===================================================
 * Thread's domain
 * ==================================================
 * For asynchronous communication 2 threads are launched,
 * one deals with getting input and sending messages,
 * the second one deals with receiving and showing them.
 */

void getandSendMessage(Socket *local, Message message, sockaddr_in remote,
                       std::string *message_text, bool *endOfLoop)
{
    //We should ead until read quit or eof
    while (!*endOfLoop) {
        std::getline(std::cin,*message_text);

        if(*message_text == "/quit" || std::cin.eof())
            *endOfLoop = true;

        if (!*endOfLoop) {
            message_text->copy(message.text, sizeof(message.text) - 1, 0);
            message.text[message_text->length()] = '\0';
            local->sendTo(reinterpret_cast<const Message&>(message));
        }
    }

}

void firsThread(Socket *local,Message message, sockaddr_in *sinRemote,
                std::string *message_text,bool *endOfLoop)
{
    try {
        getandSendMessage(&*local,message,*sinRemote,&*message_text,
                          &*endOfLoop);
    } catch (std::system_error& e) {
        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;
        *endOfLoop = true;
    }

}
//=======================================================================

void receiveAndShowMessage(Socket *socket, Message *message,
                           sockaddr_in sinRemote)
{
    while(1) {
        socket->receiveFrom(*message);

        // Mostrar el mensaje recibido en la terminal
        char* remote_ip = inet_ntoa(sinRemote.sin_addr);
        int remote_port = ntohs(sinRemote.sin_port);

        /* Para aumentar la seguridad de la aplicacion,
         * añadimos un fin de cadena que limite la lectura
         * al tamaño del buffer, por si el mensaje no está
         * delimitado de por sí
         */
        message->text[1023] = '\0';

        std::cout << "El sistema " << remote_ip << ":" << remote_port <<
        " envió el mensaje '" << message->text << "'" << std::endl;
    }

}

void secondThread(Socket *local,Message *message,
                  sockaddr_in *sinRemote, bool *endOfLoop)
{
    try {
        receiveAndShowMessage(&*local,&*message,*sinRemote);
    } catch (std::system_error& e) {
        std::cout << "Connection was over" << std::endl;
        *endOfLoop = true;
    }

}

//===============================================================

/*
 * This function will shutdown the threads
 */
void requestCancellation(std::thread& oneThread)
{
    pthread_cancel(oneThread.native_handle());
}

/*===============================================================
 * END OF THREADS DOMAIN
 *===============================================================
 */
void setupSocket(Socket *local, sockaddr_in sinLocal,
                 sockaddr_in sinRemote, int *aux)
{
    try {
         *local = Socket(sinLocal,sinRemote);

    }catch (std::system_error& e) {

        if (errno == ENOBUFS || errno == ENOMEM) {
            std::cerr << program_invocation_name << ": Not enough memory for creating Socket "
            << std::endl;
        } else {
            std::cerr << program_invocation_name << ": " << e.what()
            << std::endl;
        }

        *aux = ERR_SOCKET;    // Error. Termina el programa siempre con un valor > 0
    }
}

void startCommunication(Socket *local,sockaddr_in *sinRemote)
{
    Message message; //Estructura de mensaje
    std::string message_text(""); //String para input
    bool endOfLoop = false;

    if(local->actingLikeServer())
        local->handleConnections(&*sinRemote);
    //First thread with get input and send messages
    std::thread hilo1(&firsThread,&*local,message,&*sinRemote,&message_text,
                      &endOfLoop);
    //Second thread will recv messages and print them
    std::thread hilo2(&secondThread,&*local,&message,&*sinRemote,&endOfLoop);
    //They shall no block main thread
    hilo1.detach();
    hilo2.detach();

    while(!endOfLoop)
        usleep(25000);
    //We must finish both threads gracefully!
    requestCancellation(hilo1);
    requestCancellation(hilo2);
}

int main(void){

    int aux = SUCCESS;
    //Preparar estructura local y remoto
    sockaddr_in sinLocal = makeIpAddress("0.0.0.0",LOCALPORT);
    sockaddr_in sinRemote = makeIpAddress("0.0.0.0",REMOTEPORT);

    Socket local;

    setupSocket(&local,sinLocal,sinRemote,&aux);

    if(aux == SUCCESS) {

        startCommunication(&local,&sinRemote);
    }

    return aux;
}

