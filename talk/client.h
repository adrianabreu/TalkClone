#ifndef CLIENT_H
#define CLIENT_H
#include "socket.h"
#include "helpsignalsthreads.h"

/*
 * These are helper functions for the client setup
 * and threads launched
 */
namespace client {
    Socket setupSocket(const std::string& sinLocal,const std::string& sinRemote,
                       int port, int *aux);
    void startClient(Socket *local, const std::string& userName);

    //The first thread gets the message from input and send it
    void getandSendMessage(Socket *local,std::atomic<bool>& endOfLoop,
                           const std::string& userName);
    void firsThread(Socket& local,std::atomic<bool>& endOfLoop,
                    const std::string& userName);

    //The second thread gets the message from socket and display it
    void receiveAndShowMessage(Socket *socket);
    void secondThread(Socket& local,std::atomic<bool>& endOfLoop);

}

#endif // CLIENT_H
