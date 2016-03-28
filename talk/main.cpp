#include "socket.h"
#include <thread>
#include <pthread.h>
#include <atomic>
#include <csignal>

#define LOCALPORT 5500
#define REMOTEPORT 6000

#define SUCCESS 0
#define ERR_SOCKET 3

std::atomic<bool> endOfLoop(false);

/*
 *=====================================
 * SIGNAL HANDLING
 *=====================================
 */

void int_signal_handler(int signum)
{
    if (signum == SIGINT || signum == SIGTERM || signum == SIGHUP) {
        endOfLoop = true;
    }
}

void setSigMask(int sigAction)
{
    //If we block all signals the inherited thread will block them also
    sigset_t set;
    sigfillset(&set);
    int res = pthread_sigmask(sigAction, &set, nullptr);
    if ( res != 0 )
        throw std::system_error(errno, std::system_category(),
        "It wasn't possible to block signals receiving");
}


/*===================================================
 * Thread's domain
 * ==================================================
 * For asynchronous communication 2 threads are launched,
 * one deals with getting input and sending messages,
 * the second one deals with receiving and showing them.
 */

void getandSendMessage(Socket *local,std::atomic<bool>& endOfLoop)
{
    //We should ead until read quit or eof
    Message message;
    std::string message_text;
    while (! endOfLoop) {
        std::getline(std::cin,message_text);

        if(message_text == "/quit" || std::cin.eof())
            endOfLoop = true;

        if (!endOfLoop) {
            message_text.copy(message.text, sizeof(message.text) - 1, 0);
            message.text[message_text.length()] = '\0';
            local->sendTo(message);
        }
    }

}

void firsThread(Socket& local,std::atomic<bool>& endOfLoop)
{
    try {
        getandSendMessage(&local,endOfLoop);
    } catch (std::system_error& e) {
        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;
        endOfLoop = true;
    }

}
//=======================================================================

void receiveAndShowMessage(Socket *socket, sockaddr_in sinRemote)
{
    Message message;
    while(1) {
        socket->receiveFrom(message);

        // Display received messages
        char* remote_ip = inet_ntoa(sinRemote.sin_addr);
        int remote_port = ntohs(sinRemote.sin_port);

        /* In order to increase the security we add a \0 to limit
         * the lecture to the size of the buffer, maybe the message
         * its not delimited by default.
         */
        message.text[1023] = '\0';

        std::cout << "System " << remote_ip << ":" << remote_port <<
        " sent: '" << message.text << "'" << std::endl;
    }

}

void secondThread(Socket& local, sockaddr_in sinRemote,
                 std::atomic<bool>& endOfLoop)
{
    try {
        receiveAndShowMessage(&local,sinRemote);
    } catch (std::system_error& e) {
        std::cout << "Connection was over" << std::endl;
        endOfLoop = true;
    }

}

//===============================================================

//This function will shutdown the threads
void requestCancellation(std::thread& oneThread)
{
    pthread_cancel(oneThread.native_handle());
    oneThread.join();
}

/*===============================================================
 * End of Thread's domain
 *===============================================================
 */

Socket setupSocket(sockaddr_in sinLocal,
                 sockaddr_in sinRemote, int *aux)
{
    Socket local;
    try {
         local = Socket(sinLocal,sinRemote);

    }catch (std::system_error& e) {

        if (errno == ENOBUFS || errno == ENOMEM) {
            std::cerr << program_invocation_name
                      << ": Not enough memory for creating Socket "
                      << std::endl;
        } else {
            std::cerr << program_invocation_name << ": " << e.what()
            << std::endl;
        }

        *aux = ERR_SOCKET; //Program won't finish with 0
    }
    return local;
}

void startCommunication(Socket *local,sockaddr_in *sinRemote)
{

    if(local->actingLikeServer()) {
        try {
            local->handleConnections(&*sinRemote);
        } catch (std::system_error& e) {
            std::cerr << program_invocation_name << ": " << e.what()
            << std::endl;
        }
    }

    try {
        setSigMask(SIG_BLOCK);
    } catch (std::system_error& e) {
        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;
    }

    //First thread with get input and send messages
    std::thread hilo1(&firsThread,std::ref(*local),
                      std::ref(endOfLoop));
    //Second thread will recv messages and print them
    std::thread hilo2(&secondThread,std::ref(*local),std::ref(*sinRemote),
                      std::ref(endOfLoop));
    try {
        setSigMask(SIG_UNBLOCK);
    } catch (std::system_error& e) {
        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;
    }

    while(!endOfLoop)
        usleep(25000);
    //We must finish both threads gracefully!
    requestCancellation(hilo1);
    requestCancellation(hilo2);
}

int main(int argc, char* argv[]){

    bool help_option = false;
    bool server_option = false;
    std::string port_option;
    bool client_option = false;
    int c;
    while ((c = getopt(argc, argv, "hsc:p:")) != -1) {
        switch (c) {
            case 'h':
                help_option = 1;
            break;
            case 's':
                server_option = 1;
            break;
            case 'p':
                std::printf("opción p con valor '%s'\n", optarg);
                port_option = std::string(optarg);
            break;
            case 'c':
                client_option = 1;
            break;
            default:
                std::fprintf(stderr, "?? getopt devolvió código de error 0%o ??\n", c);
        }
    }
    int aux = SUCCESS; //Return value (0)
    sockaddr_in sinLocal = makeIpAddress("0.0.0.0",LOCALPORT);
    sockaddr_in sinRemote = makeIpAddress("0.0.0.0",REMOTEPORT);
    Socket local = setupSocket(sinLocal,sinRemote,&aux);

    if(aux == SUCCESS) {
        signal(SIGINT, &int_signal_handler);
        signal(SIGTERM, &int_signal_handler);
        signal(SIGHUP, &int_signal_handler);
        startCommunication(&local,&sinRemote);

    }
    return aux;
}

