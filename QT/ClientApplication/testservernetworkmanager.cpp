#include "testservernetworkmanager.h"

TestServerNetworkManager::TestServerNetworkManager()
{
}

void TestServerNetworkManager::startListening(quint16 port)
{
    if (this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server started!";
    }
    else
    {
        qDebug() << "Server not started!";
    }
}

void TestServerNetworkManager::incomingConnection(qintptr handle)
{
    qDebug() << "Client connected";

    QTcpSocket* socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);
    mConnections[handle] = socket;

    ConnectionHandler* connection = new ConnectionHandler(handle, socket);
    connect(connection, SIGNAL(networkReceivedData(int,unsigned char*,int)), this, SLOT(networkReceivedData(int,unsigned char*,int)));
    connection->start();
}

void TestServerNetworkManager::networkReceivedData(int connection, unsigned char* data, int size)
{
    emit this->receivedData(connection, data, size);
}

bool TestServerNetworkManager::sendData(int connectionID, const unsigned char* data, int size)
{
    QTcpSocket* socket = mConnections[connectionID];
    socket->write((const char*)data, size);
    socket->flush();

    return true;
}
