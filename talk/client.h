#ifndef CLIENT_H
#define CLIENT_H
#include "socket.h"
#include "helpsignalsthreads.h"

/*
 * These are helper functions for
 *
 */
namespace client {
    Socket setupSocket(const std::string& sinLocal,const std::string& sinRemote,
                       int port, int *aux);
    void startClient(Socket *local);

    //The first thread gets the message from input and send it
    void getandSendMessage(Socket *local,std::atomic<bool>& endOfLoop);
    void firsThread(Socket& local,std::atomic<bool>& endOfLoop);

    //The second thread gets the message from socket and display it
    void receiveAndShowMessage(Socket *socket);
    void secondThread(Socket& local,std::atomic<bool>& endOfLoop);

}

#endif // CLIENT_H
