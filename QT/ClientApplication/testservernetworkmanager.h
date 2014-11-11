#ifndef TESTSERVERTestServerNetworkManager_H
#define TESTSERVERTestServerNetworkManager_H

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "ConnectionHandler.h"

using namespace std;

class TestServerNetworkManager : public QTcpServer
{
    Q_OBJECT

private:
    map<qintptr, QTcpSocket*> mConnections;
    QThreadPool *mPool;

public:

    /**
    * Konstruktor pre triedu TestServerNetworkManager.
    */
    TestServerNetworkManager();
    ~TestServerNetworkManager(){}

    void startListening(quint16 port);

    bool sendData(int connectionID, const unsigned char* data, int size);

protected:
    void incomingConnection(qintptr handle);

signals:
    void receivedData(int connection, unsigned char* data, int size);

public slots:
    void networkReceivedData(int connection, unsigned char* data, int size);
};

#endif // TESTSERVERTestServerNetworkManager_H
