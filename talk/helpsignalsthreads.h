#ifndef HELPSIGNALSTHREADS_H
#define HELPSIGNALSTHREADS_H

#include <thread>
#include <pthread.h>
#include <csignal>
#include <atomic>

std::atomic<bool> endOfLoop(false);

#endif // HELPSIGNALSTHREADS_H
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
