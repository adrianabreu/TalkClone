#-------------------------------------------------
#
# Project created by QtCreator 2016-04-08T12:17:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = talk-gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    settingsdialog.ui
