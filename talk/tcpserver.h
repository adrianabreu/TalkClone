#ifndef TCPSERVER_H
#pragma once
#define TCPSERVER_H
#include "socket.h"
#include "helpsignalsthreads.h"
#include <vector>


/*
extern std::map<std::thread::id,Socket> clients_; //List of sockets clients, with map each thread
                           //with socket it controls
extern std::mutex clients_mutex;

extern  //List of children threads launched
                                   //from new clients connections
*/
/*
 * This class does the basic server stuff,
 * listen on a single port, and handle new connections
 */

class TCPServer : public Socket
{
private:
    std::vector<std::thread> threads_;
public:
    TCPServer();
    TCPServer(const std::string& address, int port);
    TCPServer(const TCPServer& older);

    ~TCPServer();
    //This keeps on an accept block and returns the new fd from accept
    int handleConnections(sockaddr_in *remote);
    void pushThread(std::thread& mythread);

    TCPServer& operator=(TCPServer&& older);
};

#endif // TCPSERVER_H
