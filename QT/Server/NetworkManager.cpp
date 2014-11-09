#include "NetworkManager.h"

NetworkManager::NetworkManager(Server* server)
    :mServerInstance(server) {}

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

    ConnectionHandler* connection = new ConnectionHandler(handle, mServerInstance);
    connection->start();
}
