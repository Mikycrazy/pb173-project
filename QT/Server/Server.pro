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
    Server.cpp

HEADERS += \
    Server.h \
    NetworkManager.h
