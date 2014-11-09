#include "ConnectionHandler.h"

void ConnectionHandler::run()
{
    if (!mSocketID) return;

    mSocket = new QTcpSocket();
    mSocket->setSocketDescriptor(this->mSocketID);

    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    exec();
}

void ConnectionHandler::readyRead()
{
    QByteArray data = this->mSocket->readAll();

    qDebug() << "Received data:" << data.toHex();
    this->mServer->processPacket((unsigned char*)(data.data()), this->mSocketID);
}

void ConnectionHandler::disconnected()
{
    qDebug() << "Client disconnected";
    this->deleteLater();

    exit(0);
}
