#include "client.h"

/*===================================================
 * Thread's domain
 * ==================================================
 */
void client::getandSendMessage(Socket *local,std::atomic<bool>& endOfLoop)
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

void client::firsThread(Socket& local,std::atomic<bool>& endOfLoop)
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

void client::receiveAndShowMessage(Socket *socket)
{
    Message message;
    while(1) {
        socket->receiveFrom(message);

        /* In order to increase the security we add a \0 to limit
         * the lecture to the size of the buffer, maybe the message
         * its not delimited by default.
         */
        message.text[1023] = '\0';
        std::cout << " sent: '" << message.text << "'" << std::endl;
    }

}

void client::secondThread(Socket& local, std::atomic<bool>& endOfLoop)
{
    try {
        client::receiveAndShowMessage(&local);
    } catch (std::system_error& e) {
        std::cout << "Connection was over" << std::endl;
        endOfLoop = true;
    }

}

/*===================================================
 * End of thread's domain
 * ==================================================
 */

Socket client::setupSocket(const std::string& ipLocal,const std::string& ipRemote,
                           int port, int* aux)
{
    Socket local;
    try {
         local = Socket(makeIpAddress(ipLocal,0),makeIpAddress(ipRemote,port));

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

void client::startClient(Socket *local,sockaddr_in *sinRemote)
{
    //We have to block the signals on the children
    try {
        setSigMask(SIG_BLOCK);
    } catch (std::system_error& e) {
        std::cerr << program_invocation_name << ": " << e.what()
        << std::endl;
    }

    std::thread hilo1(&firsThread,std::ref(*local),
                      std::ref(endOfLoop));

    std::thread hilo2(&secondThread,std::ref(*local),std::ref(*sinRemote),
                      std::ref(endOfLoop));

    //Now we unblock the signals on the main thread
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
