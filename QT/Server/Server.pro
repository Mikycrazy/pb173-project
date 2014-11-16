#-------------------------------------------------
#
# Project created by QtCreator 2014-11-06T13:12:05
#
#-------------------------------------------------

QT       += core
QT       += network

QT       -= gui

TARGET = Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    NetworkManager.cpp \
    Server.cpp \
    ServerTest.cpp \
    ConnectionHandler.cpp \
    logger.cpp \
    mytcpclient.cpp

HEADERS += \
    Server.h \
    NetworkManager.h \
    User.h \
    catch.hpp \
    Opcodes.h \
    ConnectionHandler.h \
    logger.h \
    mytcpclient.h
