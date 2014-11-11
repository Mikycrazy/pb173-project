#include "ConnectionHandler.h"

void ConnectionHandler::run()
{
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    qDebug() << "Handler started!";
    exec();
}

void ConnectionHandler::readyRead()
{
    QByteArray data = this->mSocket->readAll();
    qDebug() << "Received data:" << data.toHex() << "size:" << data.length();

    emit this->networkReceivedData(mSocketID, (unsigned char*)(data.data()), data.length());
}

void ConnectionHandler::disconnected()
{
    qDebug() << "Client disconnected";
    this->deleteLater();

    exit(0);
}
