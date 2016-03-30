#include "server.h"

std::map<std::thread::id,Socket> clients_; //List of sockets clients, with map each thread
                           //with socket it controls
std::mutex clients_mutex;

std::vector<std::thread> threads_;

void server::getandSendMessage(std::atomic<bool>& endOfLoop)
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
            std::unique_lock<std::mutex> lock(clients_mutex);
            server::sendAll(message);
            lock.unlock();

        }
    }

}

void server::firstThread(std::atomic<bool>& endOfLoop)
{
    try {
        getandSendMessage(endOfLoop);
    } catch (std::system_error& e) {
        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;
        endOfLoop = true;
    }

}

//Each thread should grab their own socket and read them
//we shall, copy it?
void server::receiveAndShowMessage()
{
    Message message;

    std::unique_lock<std::mutex> lock(clients_mutex);
    Socket socket = clients_[std::this_thread::get_id()];
    lock.unlock();

    while(1) {
        socket.receiveFrom(message);

        message.text[1023] = '\0';

        std::cout << " sent: '" << message.text << "'" << std::endl;
        //All of them will try to display the same time.
        //how to make safe cout?

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
        std::cout << "Connection was over" << std::endl;
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
            auto thread = std::thread(&threadReceive,std::ref(tempfd));
            threads_.push_back(std::move(thread));
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

void server::startServer(TCPServer *local)
{
    //We have to block the signals on the children
    try {
        setSigMask(SIG_BLOCK);
    } catch (std::system_error& e) {
        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;
    }

    //We will launch a thread for listen for connections
    std::thread hilo1(&server::threadListen,std::ref(local));

    //We will now launch a thread for getting user input
    std::thread hilo2(&server::firstThread, std::ref(endOfLoop));

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
    //All children threads will be closed on destructor
    //will that launch and exception?
    //to close the sockets while they're reading?
}

//The resources have been locked before, so we can
void server::sendAll(const Message& message)
{
    //C++ is like fucking awesome
    for(auto &it1 : clients_) {
        it1.second.sendTo(message);
    }
}
