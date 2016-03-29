#ifndef CLIENT_H
#define CLIENT_H
#include "socket.h"

namespace client {
    Socket setupSocket(const std::string& sinLocal,const std::string& sinRemote,
                       int port, int *aux);
    void startCommunication(Socket *local,sockaddr_in *sinRemote);

    //The first thread gets the message from input and send it
    void getandSendMessage(Socket *local,std::atomic<bool>& endOfLoop);
    void firsThread(Socket& local,std::atomic<bool>& endOfLoop);

    //The second thread gets the message from socket and display it
    void receiveAndShowMessage(Socket *socket);
    void secondThread(Socket& local,std::atomic<bool>& endOfLoop);

}

#endif // CLIENT_H
