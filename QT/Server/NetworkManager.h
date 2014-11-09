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
#include "ConnectionHandler.h"

using namespace std;

class NetworkManager : public QTcpServer
{
    Q_OBJECT

private:
    vector<qintptr> mConnections;
    QThreadPool *mPool;
    Server *mServerInstance;

public:

    /**
    * Konstruktor pre triedu NetworkManager.
    */
    NetworkManager(Server* server);
    ~NetworkManager(){}

    /**
    * Zacne pocuvat na zadanom porte
    *
    * @param port   port na ktorom ma server pocuvat
    */
    void startListening(quint16 port);

    /**
    * Odosle data zadanemu spojeniu
    *
    * @param connectionID	id spojenia
    * @param data			data na odoslanie
    * @param size			velkost dat na odoslanie
    * @return				true ak sa odoslanie podarilo, inak false
    */
    static bool sendData(int connectionID, const char* data, int size);

protected:

    /**
    * Prijme prichadzajuce spojenie, prida ho do zoznamu spojeni a zacne komunikaciu
    *
    * @param handle    id spojenia
    */
    void incomingConnection(qintptr handle);
};

#endif
