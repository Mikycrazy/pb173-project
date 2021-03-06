#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
}

NetworkManager::~NetworkManager()
{
    mConnections.clear();
}

void NetworkManager::startListening(quint16 port)
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

void NetworkManager::incomingConnection(qintptr handle)
{
    qDebug() << "Client connected";

    QTcpSocket* socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);
    mConnections[handle] = socket;

    ConnectionHandler* connection = new ConnectionHandler(handle, socket);
    connect(connection, SIGNAL(networkReceivedData(int,unsigned char*,int)), this, SLOT(networkReceivedData(int,unsigned char*,int)));
    connect(connection, SIGNAL(clientDisconnected(int)), this, SLOT(clientDisconnect(int)));
    connection->start();
}

void NetworkManager::networkReceivedData(int connection, unsigned char* data, int size)
{
    emit this->receivedData(connection, data, size);
}

bool NetworkManager::sendData(int connectionID, const unsigned char* data, int size)
{
    if(connectionID < 0)
        return false;

    QTcpSocket* socket = mConnections[connectionID];
    socket->write((const char*)data, size);

    return socket->flush();
}

void NetworkManager::clientDisconnect(int connection)
{
    emit this->disconnect(connection);
}
