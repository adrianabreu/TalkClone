# Talk - First part

[talk](https://en.wikipedia.org/wiki/Talk_%28software%29) is instant messaging program in text mode that was introduced in UNIX systems in 1983. Basically it allowed different users on different machines to talk. This system was substituted with time by [IRC](https://en.wikipedia.org/wiki/Internet_Relay_Chat) —it appeared in 1988— and it in turn gave way to the [multiple instant messages system](https://en.wikipedia.org/wiki/Comparison_of_instant_messaging_protocols) that exist nowadays.


## Compiling:

   Since we are using C++11 and threads, we need to add these directives when compiling: **-std=c++11 -pthread**
   
   For example:
   `g++ -std=c++11 -pthread socket.cpp main.cpp`

   You can also use qmake (non qt5's qmake may generate errors)
   ```
   qmake talk.pro
   make
   ``` 

   
## Basic test:
   This chat is prepared for being tested locally. For making the basics tests, two constans have been defined **LOCALPORT** y **REMOTEPORT**.

   Two versions of the program must be compiled with ports inverted.

   Examples:
   
      Program 1: LOCALPORT 3000 REMOTEPORT 4000

      Program 2: LOCALPORT 4000 REMOTEPORT 3000

   Now we can execute both programs and check if everything goes right.

## Features:

1. The program communicates through SOCKETS.
2. The communication is TCP.
3. The messages' structures are, by now, just strings.
4. Communication is also asynchronous due to the use of threads.
5. The program will enter into server mode if can't connect to remote.

## Usage:
1. Open the program, it will try to connect, if it is not possible, it will print a message as it waits for incoming connections.
2. When connected, sending messages is easy as writting them and pressing enter.
3. If another user sends the message, it will be printed on the screen after the ip of the user who sent it.
4. For existing just write `/quit` or press `CTRL+D`.

