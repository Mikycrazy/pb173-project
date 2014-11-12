#include "ConnectionHandler.h"

void ConnectionHandler::run()
{
    qDebug() << "ConnectionHandler enter";
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    qDebug() << "ConnectionHandler connect 1";
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    qDebug() << "ConnectionHandler connect 2";

    exec();
    qDebug() << "ConnectionHandler exec";
}

void ConnectionHandler::readyRead()
{
    qDebug() << "Ready read enter";
    QByteArray data = this->mSocket->readAll();
    qDebug() << "Received data:" << data.toHex() << "size:" << data.length();

    emit this->networkReceivedData(mSocketID, (unsigned char*)(data.data()), data.length());
}

void ConnectionHandler::disconnected()
{
    qDebug() << "Client disconnected";
    //this->deleteLater();

    exit(0);
}
