#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <windows.h>
#include <QRunnable>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QThread>

class ConnectionHandler : public QThread
{
    Q_OBJECT

public:
    ConnectionHandler(qintptr id, QTcpSocket* socket)
        : mSocketID(id), mSocket(socket) {}

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
    QTcpSocket* mSocket;

signals:
    void networkReceivedData(int connection, unsigned char* data, int size);

};

#endif // CONNECTIONHANDLER_H
