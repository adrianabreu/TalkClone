#include "socket.h"
#include "tcpserver.h"
#include "client.h"
#include "server.h"

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

/*===============================================================
 *  HELPER FUNCTIONS
 *===============================================================
 */
void displayHelp() {
    std::cout << "Usage mode: " << std::endl << "-h  Display this message"
              << std::endl << "-s  Enter serve mode" << std::endl
              << "-c IP  Client mode, connect remote ip" << std::endl
              << "-p PORT Listen / connect on / to that port" << std::endl;

}

void handleSignals() {
    signal(SIGINT, &int_signal_handler);
    signal(SIGTERM, &int_signal_handler);
    signal(SIGHUP, &int_signal_handler);
}

void parseArguments(bool help_option,bool server_option,std::string port_option,
                    std::string client_option, int *aux) {
    int port = 0; //A port 0 given to the bind function make it to use a random
                  //chose from the OS

    if (help_option) {
        displayHelp();
    }
    if (!port_option.isEmpty()) {
        port = stoi(port_option);
    }
    if (server_option) {

        TCPServer local = setupServer("0.0.0.0");
        //You can pass now ip and ports to the main
        if(*aux == SUCCESS) {
            handleSignals();
            startServer(&local,&sinRemote);
        }
    }
    else if (client_option != "") { //Server are also clients so servers
                                    //have priority
        Socket local = client::setupSocket("0.0.0.0",client_option,port,&*aux);
        if(*aux == SUCCESS) {
            handleSignals();
            client::startClient(&local,&sinRemote);
        }
    }
}

/*===============================================================
 * End of Thread's domain
 *===============================================================
 */

int main(int argc, char* argv[]){

    bool help_option = false;
    bool server_option = false;
    std::string port_option;
    std::string client_option;
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
                port_option = std::string(optarg);
            break;
            case 'c':
                client_option = std::string(optarg);
            break;
            default:
                std::fprintf(stderr, "?? getopt devolvió 0%o ??\n", c);
        }
    }
    int aux = SUCCESS; //Return value (0)
    parseArguments(help_option,server_option,port_option,client_option,&aux);


    return aux;
}

