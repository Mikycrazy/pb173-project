#-------------------------------------------------
#
# Project created by QtCreator 2014-11-06T11:07:52
#
#-------------------------------------------------

QT       += core
QT       += network


QT       -= gui

TARGET = ClientApplication
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ClientApplicationTests.cpp \
    NetworkManager.cpp \
    Client.cpp

HEADERS += \
    Client.h \
    CryptoManager.h \
    NetworkManager.h \
    catch.hpp
