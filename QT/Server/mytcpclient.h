#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

using namespace std;

class MyTcpClient : public QObject
{
    Q_OBJECT

private:
    QTcpSocket* mSocket;

    unsigned char* mLastReicevedData;
    int mLastReicevedDataSize;
    int mLastReicevedDataType;

public:

    /**
    * Konstruktor pre triedu NetworkManager.
    */
    explicit MyTcpClient()  {}
    ~MyTcpClient(){}

    /**
    * Zahaji spojeni tcp spojeni s nastavenou adresou a portem
    *
    * @param address	server ip
    * @param port		server port
    * @return           true ak sa spojenie podarilo
    */
    bool startConnection(QString ipAddress, quint16 port);

    /**
    * Odosle data serveru
    *
    * @param data			data na odoslanie
    * @param size			velkost dat na odoslanie
    *
    * @return				true ak sa odoslanie podarilo, inak false
    */
    bool sendData(unsigned char* data, int size);

    unsigned char* getLastData() {return mLastReicevedData; }

    int getLastDataSize() { return mLastReicevedDataSize; }

    int getLastDataType() { return mLastReicevedDataType; }

public slots:

    /**
    * Vykona potrebne akcie v pripade odpojenia od servera
    */
    void disconnected();

    /**
    * Prijme data od servera a posunie na spracovanie
    */
    void receiveData();

signals:
    void networkReceivedData(unsigned char* data, int size);
};


#endif // MYTCPCLIENT_H
