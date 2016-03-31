#include "server.h"
#include <mutex>
#include <thread>
#include <map>

std::map<std::thread::id,Socket> clients_; //List of sockets clients,

std::mutex clients_mutex;

void server::getandSendMessage(std::atomic<bool>& endOfLoop,
                               const std::string& userName)
{
    //We should ead until read quit or eof
    Message message;
    std::string message_text;
    while (! endOfLoop) {
        std::getline(std::cin,message_text);

        if(message_text == "/quit" || std::cin.eof())
            endOfLoop = true;

        if (!endOfLoop) {
            userName.copy(message.username,sizeof(message.username) - 1 ,0);
            message.username[userName.length()] = '\0';
            message_text.copy(message.text, sizeof(message.text) - 1, 0);
            message.text[message_text.length()] = '\0';
            std::unique_lock<std::mutex> lock(clients_mutex);
            server::sendAll(message,std::this_thread::get_id());
            lock.unlock();

        }
    }

}

void server::firstThread(std::atomic<bool>& endOfLoop,
                         const std::string& userName)
{
    try {
        getandSendMessage(endOfLoop,userName);
    } catch (std::system_error& e) {
        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;
        endOfLoop = true;
    }

}

void server::receiveAndShowMessage()
{
    Message message;

    std::unique_lock<std::mutex> lock(clients_mutex);
    Socket socket = clients_[std::this_thread::get_id()];
    lock.unlock();

    while(1) {
        socket.receiveFrom(message);
        message.username[15]= '\0';
        message.text[1023] = '\0';

        std::unique_lock<std::mutex> lock(clients_mutex);
        std::cout << message.username << " sent: '" << message.text << "'"
                  << std::endl;
        server::sendAll(message,std::this_thread::get_id());
        lock.unlock();
    }

}

void server::threadReceive(int& tempfd)
{
    //So technically we have a socket here, we have the fd,
    //lets build the socket and insert into the map
    // so the others threads could send messages to it
    //for avoiding problems we will lock the resource
    try {
        std::unique_lock<std::mutex> lock(clients_mutex);
        clients_[std::this_thread::get_id()] = Socket(tempfd);
        lock.unlock();
        server::receiveAndShowMessage();
    } catch (std::system_error& e) {
        //probably the socket is over here
        //std::cout << "Error receiving" << std::endl;
        //so what we do?
        //we finish
        std::unique_lock<std::mutex> lock(clients_mutex);
        clients_.erase(std::this_thread::get_id());
        lock.unlock();
        //Here we got to destroy the socket and the thread.
        //How?
    }
}

void server::threadListen(TCPServer *local)
{
    while(!endOfLoop) {
        try {
            sockaddr_in sinRemote = {};
            int tempfd = local->handleConnections(&sinRemote);
            auto mythread = std::thread(&threadReceive,std::ref(tempfd));
            local->pushThread(mythread);
        } catch (std::system_error& e) {
            std::cerr << program_invocation_name << ": " << e.what()
            << std::endl;
        }

    }
}

TCPServer server::setupServer(const std::string& ipLocal, int port, int* aux)
{
    TCPServer local;
    try {
         local = TCPServer(ipLocal,port);

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

void server::startServer(TCPServer *local,const std::string& userName)
{
    //We have to block the signals on the children
    try {
        setSigMask(SIG_BLOCK);
    } catch (std::system_error& e) {
        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;
    }

    //We will launch a thread for listen for connections
    std::thread hilo1(&server::threadListen, std::ref(local));

    //We will now launch a thread for getting user input
    std::thread hilo2(&server::firstThread, std::ref(endOfLoop),
                      std::ref(userName));

    //Now we unblock the signals on the main thread
    try {
        setSigMask(SIG_UNBLOCK);
    } catch (std::system_error& e) {
        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;
    }

    while(!endOfLoop)
        usleep(25000);

    //We must close all the sockets and finish all the threads
    requestCancellation(hilo1);
    requestCancellation(hilo2);
}

//The resources have been locked before, so we can send all
void server::sendAll(const Message& message, std::thread::id senderId)
{
    for(auto &it1 : clients_) {
        if (it1.first != senderId)
            it1.second.sendTo(message);
    }
}
