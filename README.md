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

## What's new on Talk 1.2? :

Most people found terminal uncorfortable, so in Talk 1.2 you can use a GUI for interact with others!

* The whole code have been redistributed.
* A server mode have been implemented.
* Now messages are send to the multiple users connected to the same server.
* Support to terminal arguments.
* Usernames allowed.
* Signal handling.
* Messages are saved on a history.

## Arguments for launching :

The program now requires of bash arguments, please check them below:

* **-h --help**
  Display help message.
* **-s --server**
  Enter server mode.
* **-c --client IP**
  Enter client mode and connect that ip (port required).
* **-p --port PORT**
  Specify port for listening / connecting.
* **-u --user USER**
  Specify sender's name.


## Basic test:

If you want to test it locally just follow this steps:

1. Compile the program.
1. Launch an instance in server mode and listen to a free port (**./ talk -s -p 8000**).
1. Launch another instance in client mode with ip 127.0.0.1 and the same port (**-c 127.0.0.1 -p 8000**).
1. Start messaging!

## Features:

1. The program communicates through SOCKETS.
2. The communication is TCP.
3. The messages' structures are, by now, just strings.
4. Communication is also asynchronous due to the use of threads.
5. You can host your own server, and all the users connected to you will see all the messages.
6. Usernames are allowed!
7. Also supports system's signals (sigterm, sighup...).
8. History implemented under ~/.talk !

## Usage:

### I want to be the server:
1. Compile.
2. Execute talk with **-s**.
3. Tell your friends your IP and the port talk showed on the screen.
4. Talk with every one.
4. For existing just write `/quit` or press `CTRL+D`.

### I want to be the client:
1. Compile.
2. Execute talk with the IP and the PORT your friend told you. **-c IP -p PORT**.
3. Talk with every one!
4. For exiting just write `/quit` or press `CTRL+D`.

### What is that username?:
If you didn't specified your username with **-u USERNAME** when launching talk, 
it will be the one on your $USER environment variable.

### What if I forget any command?:
Feel free to check it on **-h --help**

### Are messages encrypted?:
No, I'm following whatsapp babysteps.

### History
The messages are saved on files. The history size will be as much 1MB. (It works as a circular buffer). 
You can found your histories on **~/.talk/username.log**

### How is the GUI?
Here are a few screenshots, hope you like them:
![Main window](assets/talk-ing.png?raw=true "Main window")

![Setup dialog](assets/setupdialog.png?raw=true "Setup dialog")

![About dialog](assets/aboutdialog.png?raw=true "About dialog")

### Do I HAVE TO use the GUI?
No, I know that there are terminal lovers out there (as the truth), you can keep using talk as always ;)
