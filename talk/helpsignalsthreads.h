#ifndef HELPSIGNALSTHREADS_H
#define HELPSIGNALSTHREADS_H
#pragma once
#include <thread>
#include <pthread.h>
#include <csignal>
#include <atomic>
#include <system_error>

#define SUCCESS 0
#define ERR_SOCKET 3

extern std::atomic<bool> endOfLoop;

/*
 * These are helping functions common to both
 * server and client side.
 */

//Signal helping
void int_signal_handler(int signum);
void setSigMask(int sigAction);

//Thread
//This function will shutdown the threads
void requestCancellation(std::thread& oneThread);

#endif // HELPSIGNALSTHREADS_H
