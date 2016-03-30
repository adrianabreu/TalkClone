#ifndef SERVER_H
#define SERVER_H
#include "tcpserver.h"
/*
 * These are helper functions for setting up the
 * server and launch the new threads
 * also it controls the sendall, all servers have
 * 3 + n thredas
 * main + listen + input + n * clientsthreads
 */
namespace server {
    TCPServer setupServer(const std::string& ipAdress, int port, int* aux);
     //launches a thread that will keep on accept
    void threadListen(TCPServer* local);
    //function for each children thread
    void threadReceive(int& tempfd);
    void receiveAndShowMessage();
    void startServer(TCPServer *local); //main program
    void sendAll(const Message& message);
    //The first thread gets the message from input and send it
    void getandSendMessage(std::atomic<bool>& endOfLoop);
    void firstThread(std::atomic<bool>& endOfLoop);
    void shutdown();
}
#endif // SERVER_H
