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
    Client.cpp \
    logger.cpp \
    myserver.cpp

HEADERS += \
    Client.h \
    CryptoManager.h \
    NetworkManager.h \
    catch.hpp \
    Opcodes.h \
    logger.h \
    myserver.h


unix|win32: LIBS += -L$$PWD/polarssl-1.3.8/lib/ -lPolarSSL

INCLUDEPATH += $$PWD/polarssl-1.3.8/include
DEPENDPATH += $$PWD/polarssl-1.3.8/include
