#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <windows.h>
#include <QRunnable>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QThread>
#include "Server.h"

class ConnectionHandler : public QThread
{
    Q_OBJECT

public:
    ConnectionHandler(qintptr socketID, Server* server)
        : mSocketID(socketID), mServer(server) {}

    void run();

public slots:

    /**
    * Prijme data od aktivneho spojenia a odosle na spracovanie serveru
    */
    void readyRead();

    /**
    * Ukonci spojenie a pripadne odhlasi aktivneho uzivatela
    */
    void disconnected();

private:
    qintptr mSocketID;
    Server* mServer;
    QTcpSocket* mSocket;
};

#endif // CONNECTIONHANDLER_H
