#include "NetworkManager.h"

bool NetworkManager::startConnection(QString ipAddress, quint16 port)
{
    this->mSocket = new QTcpSocket();

    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));

    qDebug() << "Connecting to" << ipAddress << "at port" << port;

    mSocket->connectToHost(ipAddress, port);
    if (!mSocket->waitForConnected(10000))
    {
        qDebug() << "Error: " << mSocket->errorString();
        return false;
    }
    else
    {
        qDebug() << "Connected!";
        return true;
    }
}

bool NetworkManager::sendData(unsigned char *data, int size)
{
    mSocket->write((const char*)data, size);
    mSocket->flush();

    return mSocket->waitForBytesWritten(10000);
}

void NetworkManager::disconnected()
{
    qDebug() << "Server disconnected!";
}

void NetworkManager::receiveData()
{
    QByteArray data = mSocket->readAll();
    qDebug() << "Received data from server:" << data.data();
}
