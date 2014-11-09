#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

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

using namespace std;

class NetworkManager : public QObject
{
    Q_OBJECT

private:
    QTcpSocket* mSocket;

public:

    /**
    * Konstruktor pre triedu NetworkManager.
    */
    explicit NetworkManager()  {}
    ~NetworkManager(){}

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

public slots:

    /**
    * Vykona potrebne akcie v pripade odpojenia od servera
    */
    void disconnected();

    /**
    * Prijme data od servera a posunie na spracovanie
    */
    void receiveData();
};

#endif
