#include "socket.h"
#include "tcpserver.h"
#include "client.h"
#include "server.h"
#include "helpsignalsthreads.h"

#define LOCALPORT 5500
#define REMOTEPORT 6000

std::atomic<bool> endOfLoop(false);

void handleSignals() {
    signal(SIGINT, &int_signal_handler);
    signal(SIGTERM, &int_signal_handler);
    signal(SIGHUP, &int_signal_handler);
}

void displayHelp()
{
    std::cout << "Usage mode: " << std::endl
              << "-h: Display this message " << std::endl
              << "-s: Enter server mode" << std::endl
              << "-c IP: Connect to that ip " << std::endl
              << "-p PORT: listen/connect on port" << std::endl;
}

void parseArguments(bool help_option,bool server_option,std::string port_option,
                    std::string client_option, int *aux)
{
    int port = 0; //A port 0 given to the bind function make it to use a random
                  //chose from the OS

    if (help_option) {
        displayHelp();
    }
    if (!port_option.empty()) {
        port = stoi(port_option);
    }
    if (server_option) {

        TCPServer local = server::setupServer("0.0.0.0",port,*&aux);
        if(*aux == SUCCESS) {
            handleSignals();
            server::startServer(&local);
        }
    }
    else if (client_option != "") { //Server are also clients so servers
                                    //have priority
        Socket local = client::setupSocket("0.0.0.0",client_option,port,&*aux);
        if(*aux == SUCCESS) {
            handleSignals();
            client::startClient(&local);
        }
    }
}

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
                std::fprintf(stderr, "getopt devolvió 0%o \n", c);
        }
    }
    int aux = SUCCESS; //Return value (0)
    parseArguments(help_option,server_option,port_option,client_option,&aux);

    return aux;
}

