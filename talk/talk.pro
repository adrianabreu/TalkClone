TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += thread

SOURCES += main.cpp \
    socket.cpp \
    tcpserver.cpp

HEADERS += \
    socket.h \
    tcpserver.h
